/*
v0.1
- First version.
*/
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <filesystem>
#include <fstream>
#include "minidebug.h"

#define ARG(x) (!std::strcmp(argv[i], (x)))
#define NOT_IMPLEMENTED printf("Not implemented.\n");

enum class OutputMode {
    c,
    hc,
    hcx,
    cx
};

struct {
    std::string out_prefix_h = "";
    std::string out_suffix_h = ".h";
    std::string out_prefix_c = "";
    std::string out_suffix_c = ".c";
    std::string out_prefix_x = "";
    std::string out_suffix_x = ".x";
    std::string include_prefix = "";
    bool no_bad_chars = true;
    OutputMode out_mode = OutputMode::hc;
    bool dry_run = false;
} Config;

std::vector<std::filesystem::path> in_paths; // There must be a better container than a dynamic array (e.g. list), but I have no internet to look them up.
std::filesystem::path out_dir_path;

const char help_text[] =
R"(bin2hc v0.1 "Anything but IAMT 4" Edition
Usage: bin2hc.exe <options>... [--] <in_file>... <out_dir>
-- marks the end of options (optional).
Options:
    <prefix>filename<suffix>.
    'c' is a source file, 'h' is a header, 'x' is array data file to {#include ".x"}.
    --prefix_<c|h|x> <str>      Override prefix (text can be ANSI only).
                                    Defaults for c, h, x: "", "", "".
    --suffix_<c|h|x> <str>      Override suffix.
                                    Defaults for c, h, x: ".c", ".h", ".x"
    --allow-bad-chars           Don't replace characters to make the name valid in C (space, . -> _).
                                    Replacement does not affect prefix&suffix.
    --format[s] <c|hc|hcx|cx>   Which combination of header, source and array data is needed.
                   ^ Default
    --include-prefix <str>      For hc, cx, hcx, prefix to add in `#include <<iprefix>file.h>`
    --dry, --dry-run            Don't touch the files, show what would happen if this command was run.    
Examples:
    bin2hc.exe file.png .
        Creates file_png.c + file_png.h in the same directory with `unsigned char file_png[file_png_size]`.
    bin2hc.exe --prefix_c "data_" --suffix_c ".h" --formats c ananas.ogg banana.svg ./assets
        assets/data_ananas_ogg.h; assets/data_banana_svg.h
    bin2hc.exe --dry -- --dry
        ./__dry.h; ./__dry.c

    'The one from CMake wasn't enough'
)";

void ShowHelp() {
    printf("%s", help_text);
}

const char OUT_pragma[] = "#pragma once\n";
const char OUT_include_common[] = "#include <stdint.h>\n";
const char OUT_ifdef_beg[] = 
R"(#ifdef __cplusplus
extern "C" {
#endif
)";
const char OUT_ifdef_end[] = 
R"(#ifdef __cplusplus
}
#endif
)";
const char OUT_size_t[] = "const size_t ";
const char OUT_datatype[] = "const unsigned char ";

bool ProcessFile(std::filesystem::path& in_path) {
    std::string corrected_name = in_path.filename().string(); // .stem is filename - extension.
    if (corrected_name.empty()) {
        printf("Empty name (how did you get here?)\n");
        return false;
    }
    miniprintfvalloc("%s", corrected_name.c_str());
    if (Config.no_bad_chars) {
        miniprintflocln();
        if (corrected_name.at(0) >= '0' && corrected_name.at(0) <= '9') {
            corrected_name.insert(corrected_name.begin(), '_');
        }
        size_t bad = 0; // I'd like to know why I can't leave it in 'while'.
        while ((bad = corrected_name.find_first_not_of("$0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz")) != std::string::npos) {
            corrected_name.at(bad) = '_';
        }
    }
    miniprintflocln();

    std::filesystem::path out_name_h = Config.out_prefix_h + corrected_name + Config.out_suffix_h;
    std::filesystem::path out_name_c = Config.out_prefix_c + corrected_name + Config.out_suffix_c;
    std::filesystem::path out_name_x = Config.out_prefix_x + corrected_name + Config.out_suffix_x;
    uintmax_t out_data_size = std::filesystem::file_size(in_path);
    std::string out_data_name = out_name_c.stem().string();
    miniprintflocln();
    std::ifstream ifile(in_path, std::ios::binary);
    if (!ifile) {
        std::cout << "Could not read " << in_path.string() << ".\n";
        return false;
    }

    miniprintfvalloc("%d", Config.out_mode);
    switch (Config.out_mode) {
        case OutputMode::hc: {
            std::ofstream ofile_h;
            if (Config.dry_run) { ofile_h.basic_ios<char>::rdbuf(std::cout.rdbuf()); }
            else {
                ofile_h.open(out_dir_path / out_name_h);
                if (!ofile_h) { std::cout << "Could not write " << (out_dir_path / out_name_h).string() << ".\n"; }
            }
            ofile_h << (Config.dry_run ? "--[ " + (out_dir_path/out_name_h).string() + " ]------\n" : "") <<
            OUT_pragma << 
            OUT_include_common <<
            OUT_ifdef_beg <<
            OUT_size_t << out_data_name << "_size = " << out_data_size << ";\n" <<
            "extern " << OUT_datatype << out_data_name << "[" << out_data_size << "];\n";
            
            
            ofile_h << OUT_ifdef_end <<
            (Config.dry_run ? "--[ END OF FILE ]-------\n" : "");
            ofile_h.close();
            // No brake here!
        }
        case OutputMode::c: {
            std::ofstream ofile_c;
            // Stackoverflow says that this does not work unless the file has been opened beforehand. However, this does. xd
            if (Config.dry_run) { ofile_c.basic_ios<char>::rdbuf(std::cout.rdbuf()); }
            // if (Config.dry_run) { ofile_c.set_rdbuf(std::cout.rdbuf()); }
            else {
                ofile_c.open(out_dir_path / out_name_c);
                if (!ofile_c) { std::cout << "Could not write " << (out_dir_path / out_name_c).string() << ".\n"; }
            }
            ofile_c << (Config.dry_run ? "--[ " + (out_dir_path/out_name_c).string() + " ]------\n" : "") <<
            (Config.out_mode == OutputMode::hc ? "" : OUT_pragma) << 
            (Config.out_mode == OutputMode::hc ? "#include \"" + Config.include_prefix + out_name_h.string() + "\"\n": OUT_include_common) <<
            OUT_ifdef_beg;
            if (Config.out_mode != OutputMode::hc){
                ofile_c <<OUT_size_t << out_data_name << "_size = " << out_data_size << ";\n";
            }
            ofile_c << OUT_datatype << out_data_name << "[" << out_data_size << "] = {\n    ";
            
            ofile_c << std::hex << std::showbase;
            uint8_t byte = ifile.get();
            for (uintmax_t cnt = 0; !ifile.eof(); cnt++, byte = ifile.get()) {
                if (!Config.dry_run || out_data_size < 10) {
                    ofile_c << (cnt == 0 ? "" : ", ");
                    ofile_c << static_cast<uint16_t>(byte);
                }
                else {
                    if (out_data_size <= 11 || (cnt < 5 || cnt >= out_data_size - 5)) {
                        ofile_c << (cnt == 0 ? "" : ", ");
                        ofile_c << static_cast<uint16_t>(byte);
                    }
                    else if (cnt == 5) {
                        ofile_c << ", ...";
                    }
                }
            }
            ofile_c << std::noshowbase << std::dec;
            ofile_c << "\n};\n" << OUT_ifdef_end <<
            (Config.dry_run ? "--[ END OF FILE ]-------\n" : "");
            ofile_c.close();
            std::cout << "Converted " << in_path.string() << ".\n";
            break;
        }
        case OutputMode::cx: {
            NOT_IMPLEMENTED
            break;
        }
        case OutputMode::hcx:{
            NOT_IMPLEMENTED
            break;
        }
        default:
            return false;
            break;
    }
    return true;
}

bool ParseArgs(int& argc, char** &argv) {
    bool no_more_opts = false;
    if (argc <= 1) { ShowHelp(); return false; }
    for (int i = 1; i < argc; i++) {
        if (!no_more_opts) {
            if (ARG("--help") || ARG("-h")) {
                ShowHelp();
                return false;
            }
            else if (ARG("--prefix_h")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --prefix_h.\n");
                    return false;
                }
                Config.out_prefix_h = std::string(argv[++i]);
                continue;
            }
            else if (ARG("--suffix_h")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --suffix_h.\n");
                    return false;
                }
                Config.out_suffix_h = std::string(argv[++i]);
                continue;
            }
            else if (ARG("--prefix_c")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --prefix_c.\n");
                    return false;
                }
                Config.out_prefix_c = std::string(argv[++i]);
                continue;
            }
            else if (ARG("--suffix_c")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --suffix_c.\n");
                    return false;
                }
                Config.out_suffix_c = std::string(argv[++i]);
                continue;
            }
            else if (ARG("--prefix_x")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --prefix_x.\n");
                    return false;
                }
                Config.out_prefix_x = std::string(argv[++i]);
                continue;
            }
            else if (ARG("--suffix_x")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --suffix_x.\n");
                    return false;
                }
                Config.out_suffix_x = std::string(argv[++i]);
                continue;
            }
            
            else if (ARG("--formats") || ARG("--format")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --formats.\n");
                    return false;
                }
                std::string arg(argv[++i]);
                bool h = arg.find("h") != std::string::npos;
                bool c = arg.find("c") != std::string::npos;
                bool x = arg.find("x") != std::string::npos;
                if (c && !h && !x) { Config.out_mode = OutputMode::c; }
                else if (c && h && !x) { Config.out_mode = OutputMode::hc; }
                else if (c && h && x) { Config.out_mode = OutputMode::hcx; }
                else if (c && !h && x) { Config.out_mode = OutputMode::cx; }
                else if (!c && h && !x) { printf("Use `--formats c --out-suffix_c \".h\"` instead of `--formats h`.\n"); return false; }
                else { printf("Unsupported combination for --formats: %s%s%s\n", (c ? "c" : ""), (h ? "h" : ""), (x ? "x" : "")); }
                continue;
            }
            else if (ARG("--include-prefix")) {
                if (i+1 >= argc) {
                    printf("Not enough arguments for --include-prefix.\n");
                    return false;
                }
                Config.include_prefix = std::string(argv[++i]);
                continue;
            }
            else if (ARG("--allow-bad-chars")) {
                Config.no_bad_chars = false;
                continue;
            }
            else if (ARG("--dry") || ARG("--dry-run")) {
                Config.dry_run = true;
                continue;
            }
            else if (ARG("--")) {
                no_more_opts = true;
                continue;
            }
        }
        // End of options (--)

        std::filesystem::path path(argv[i]);
        if (i < argc-1) { // Input
            if (std::filesystem::is_directory(path)) {
                printf("Input directories are not supported yet.\n");
                return false;
            }
            else if (!std::filesystem::is_empty(path)) {
                in_paths.push_back(path);
                continue;
            }
        }
        else { // Output, last arg
            if (std::filesystem::is_directory(path)) {
                out_dir_path = path;
                continue;
            }
            else {
                printf("Output must be a directory.\n");
                return false;
            }
        }        
    }
    if (in_paths.empty() || out_dir_path.empty()) {
        printf("Input/output argument is missing.\n");
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    if (ParseArgs(argc, argv)) {
        if (Config.dry_run) { printf("Dry run!\n"); }
        for (auto x : in_paths) { 
            miniprintflocln();
            std::cout << "Processing " << x.string() << ".\n";
            if (!ProcessFile(x)) { break; }
        }
    }

    return 0;
}