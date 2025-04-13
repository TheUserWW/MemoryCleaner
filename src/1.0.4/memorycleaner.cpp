// memorycleaner.cpp
#include "memorycleaner.h"
#include <QLibrary>


bool MemoryCleaner::cleanWorkingSet()
{
    return EmptyWorkingSet(GetCurrentProcess());
}

bool MemoryCleaner::cleanSystemWorkingSet()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (pe32.th32ProcessID == 0) {
            continue;
        }

        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA | PROCESS_VM_OPERATION | PROCESS_VM_READ,
            FALSE,
            pe32.th32ProcessID);

        if (hProcess != NULL) {
            EmptyWorkingSet(hProcess);
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return true;
}

bool MemoryCleaner::cleanStandbyList(bool clearLowPriority) {
    typedef NTSTATUS(NTAPI* pNtSetSystemInformation)(INT, PVOID, ULONG);

    QLibrary ntdll("ntdll.dll");
    auto NtSetSystemInformation = (pNtSetSystemInformation)ntdll.resolve("NtSetSystemInformation");

    if (!NtSetSystemInformation) return false;  // 函数不存在时直接返回

    SYSTEM_MEMORY_LIST_COMMAND command = clearLowPriority ?
                                             MemoryPurgeLowPriorityStandbyList : MemoryPurgeStandbyList;

    return NT_SUCCESS(NtSetSystemInformation(
        SystemMemoryListInformation,
        &command,
        sizeof(SYSTEM_MEMORY_LIST_COMMAND)
        ));
}

bool MemoryCleaner::cleanModifiedPageList() {
    typedef NTSTATUS(NTAPI* pNtSetSystemInformation)(INT, PVOID, ULONG);
    QLibrary ntdll("ntdll.dll");
    auto NtSetSystemInformation = (pNtSetSystemInformation)ntdll.resolve("NtSetSystemInformation");
    if (!NtSetSystemInformation) return false;

    SYSTEM_MEMORY_LIST_COMMAND command = MemoryPurgeModifiedPageList;
    return NT_SUCCESS(NtSetSystemInformation(
        SystemMemoryListInformation,
        &command,
        sizeof(SYSTEM_MEMORY_LIST_COMMAND)
        ));
}

bool MemoryCleaner::cleanCombinePageList() {
    typedef NTSTATUS(NTAPI* pNtSetSystemInformation)(INT, PVOID, ULONG);
    QLibrary ntdll("ntdll.dll");
    auto NtSetSystemInformation = (pNtSetSystemInformation)ntdll.resolve("NtSetSystemInformation");
    if (!NtSetSystemInformation) return false;

    SYSTEM_MEMORY_LIST_COMMAND command = MemoryPurgeCombinedPageList;
    return NT_SUCCESS(NtSetSystemInformation(
        SystemMemoryListInformation,
        &command,
        sizeof(SYSTEM_MEMORY_LIST_COMMAND)
        ));
}


bool MemoryCleaner::cleanAllProcessesWorkingSet()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (pe32.th32ProcessID == 0) continue;

        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA | PROCESS_VM_OPERATION | PROCESS_VM_READ,
            FALSE,
            pe32.th32ProcessID);
        if (hProcess) {
            EmptyWorkingSet(hProcess);
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return true;
}

bool MemoryCleaner::getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS)
{
    totalWS = 0;
    sharedWS = 0;
    privateWS = 0;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (pe32.th32ProcessID == 0) continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess) {
            PROCESS_MEMORY_COUNTERS_EX pmc;
            if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                totalWS += pmc.WorkingSetSize;
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    // 简化计算：假设共享工作集占总工作集的30%
    sharedWS = totalWS * 0.3;
    privateWS = totalWS - sharedWS;

    return true;
}
