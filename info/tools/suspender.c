#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

typedef NTSTATUS (NTAPI *NtSuspendProcessPtr)(HANDLE ProcessHandle);
typedef NTSTATUS (NTAPI *NtResumeProcessPtr)(HANDLE ProcessHandle);

NtSuspendProcessPtr NtSuspendProcess = NULL;
NtResumeProcessPtr  NtResumeProcess = NULL;

HANDLE get_process_handle(const char* name) {
    PROCESSENTRY32 entry = { .dwSize = sizeof(PROCESSENTRY32) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(Process32First(snapshot, &entry) != TRUE) {
        CloseHandle(snapshot);
        return NULL;
    }
    HANDLE process = NULL;
    while(Process32Next(snapshot, &entry) == TRUE) {
        if(stricmp(entry.szExeFile, name) == 0) {
            process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
            break;
        }
    }
    CloseHandle(snapshot);
    return process;
}

void suspend_if_exists(const char* name) {
    HANDLE tmp = get_process_handle(name);
    if(tmp != NULL) {
        NTSTATUS stat = NtSuspendProcess(tmp);
        printf("Suspended: %s with status %d\n", name, stat);
        printf("Press Enter to resume process");
        getchar();
        NtResumeProcess(tmp);
        CloseHandle(tmp);
        exit(0);
    }
}

int main() {
    NtSuspendProcess = (NtSuspendProcessPtr)GetProcAddress(GetModuleHandle("NTDLL"), "NtSuspendProcess");
    NtResumeProcess = (NtSuspendProcessPtr)GetProcAddress(GetModuleHandle("NTDLL"), "NtResumeProcess");
    for(;;) {
        suspend_if_exists("RemoteVstPlugin.exe");
        suspend_if_exists("RemoteVstPlugin32.exe");
    }
    return 0;
}