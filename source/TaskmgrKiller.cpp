#include "../include/TaskmgrKiller.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>

namespace TaskmgrKiller {
    int PrintProcessNameAndID(DWORD processID, const CHAR* name) {
        CHAR szProcessName[MAX_PATH] = "<unknown>";

        // Get a handle to the process.

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, processID);

        // Get the process name.

        if (NULL != hProcess) {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(CHAR));
            }
        }
        //fprintf_s(stdout, "%s\n", szProcessName);
        if (strcmp(szProcessName, name) == 0) // right process
        {
            fprintf_s(stdout, "Found the process!\n");
            CloseHandle(hProcess);
            return 1;
        }

        // Release the handle to the process.
        CloseHandle(hProcess);
        return 0;
    }

    int KillTaskmgr() {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        unsigned int i;

        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
            return 1;
        }
        // Calculate how many process identifiers were returned.
        cProcesses = cbNeeded / sizeof(DWORD);
        // Print the name and process identifier for each process.
        for (i = 0; i < cProcesses; i++) {
            if (aProcesses[i] != 0) {
                if (PrintProcessNameAndID(aProcesses[i], "Taskmgr.exe")) {
                    printf("Found!\n");
                    const auto prc = OpenProcess(PROCESS_TERMINATE, false, aProcesses[i]);
                    if (!TerminateProcess(prc, 1)) {
                        fprintf_s(stdout, "Could not kill taskmgr.exe\n");
                    }
                    CloseHandle(prc);
                }
            }
        }
        return 0;
    }

}