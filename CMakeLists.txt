cmake_minimum_required(VERSION 4.2)
project(iamt_april_fools LANGUAGES C CXX)

message("Options:")
option(VENDORED_LIBRARIES "Use libraries from the project 'external' folder, copied or symlinked." OFF)
message("  VENDORED_LIBRARIES    ${VENDORED_LIBRARIES}")
option(USE_TTF_FONTS "Use SDL_ttf for TTF fonts. Raster fallback is used if disabled." OFF)
message("  USE_TTF_FONTS         ${USE_TTF_FONTS}")
option(MALICIOUS "Apply anti-alt-tab, always on top and other fun stuff." OFF)
message("  MALICIOUS             ${MALICIOUS}")
option(CI72_EMBEDDED_ASSETS "Assets are compiled .c files instead of assets folder." OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if (MSVC)
    add_compile_options("/utf-8")
endif()


if (VENDORED_LIBRARIES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL3 EXCLUDE_FROM_ALL)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL3_image EXCLUDE_FROM_ALL)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL3_mixer EXCLUDE_FROM_ALL)
    if (USE_TTF_FONTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL3_ttf EXCLUDE_FROM_ALL)
    endif()
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
    find_package(SDL3_image REQUIRED CONFIG)
    find_package(SDL3_mixer REQUIRED CONFIG)
    if (USE_TTF_FONTS)
    find_package(SDL3_ttf REQUIRED CONFIG)
    endif()
endif()

add_subdirectory("source/bin2hc" EXCLUDE_FROM_ALL)



set(iamt_april_fools_SOURCES
source/BulletSpawner.cpp
source/ChickenHandler.cpp
source/CI72Locker.cpp
source/DickClark.cpp
source/LevelLoader.cpp
source/main.cpp
source/MusicPlayer.cpp
source/Player.cpp
source/Texture.cpp
# CI72Locker.rc
)
set(iamt_april_fools_SOURCES_ASSETS
source/assets/mus_music3_mid.c
source/assets/snd_cash_ogg.c
source/assets/snd_chicken_die_ogg.c
source/assets/snd_chicken_hit_wav.c
source/assets/snd_dc_death_wav.c
source/assets/snd_dc_hit_wav.c
source/assets/snd_dc_teleport_wav.c
source/assets/snd_gunsh_wav.c
source/assets/snd_oo_wav.c
source/assets/TextureAtlas_png.c
)

add_executable(${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} bin2hc)
# target_include_directories("include")
target_sources(${PROJECT_NAME} PRIVATE ${iamt_april_fools_SOURCES} $<$<BOOL:CI72_EMBEDDED_ASSETS>:${iamt_april_fools_SOURCES_ASSETS}>)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer
    # SDL3_ttf::SDL3_ttf # FIXME привязать к выражению генератора.
)
target_compile_definitions(${PROJECT_NAME} PUBLIC VME_EMBEDDED_ASSETS)
if (USE_TTF_FONTS) # FIXME Переделать с выражением генератора.
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_TTF_FONTS=1)
else()
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_TTF_FONTS=0)
endif()
if (MALICIOUS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC MALICIOUS)
endif()
if (CI72_EMBEDDED_ASSETS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CI72_EMBEDDED_ASSETS)
endif()

if (MINGW)
    message(STATUS "GNU compiler, added -static.")
    target_link_options(${PROJECT_NAME} PRIVATE -static)
endif()
if (WIN32)
    message(STATUS "DLLs will be copied to the build dir.")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
       COMMAND "${CMAKE_COMMAND}" -E create_symlink "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    )
    # how to force this to run pre_build on non-VS generators?
#[[
    add_custom_command(TARGET ${PROJECT_NAME} DEPENDS ${iamt_april_fools_SOURCES_ASSETS} COMMAND
        bin2hc --format hc
        ${CMAKE_SOURCE_DIR}/assets/mus_music3.mid
        ${CMAKE_SOURCE_DIR}/assets/snd_cash.ogg
        ${CMAKE_SOURCE_DIR}/assets/snd_chicken_die.ogg
        ${CMAKE_SOURCE_DIR}/assets/snd_chicken_hit.wav
        ${CMAKE_SOURCE_DIR}/assets/snd_dc_death.wav
        ${CMAKE_SOURCE_DIR}/assets/snd_dc_hit.wav
        ${CMAKE_SOURCE_DIR}/assets/snd_dc_teleport.wav
        ${CMAKE_SOURCE_DIR}/assets/snd_gunsh.wav
        ${CMAKE_SOURCE_DIR}/assets/snd_oo.wav
        ${CMAKE_SOURCE_DIR}/assets/TextureAtlas.png

        ${CMAKE_SOURCE_DIR}/assets
    )
]]
endif()