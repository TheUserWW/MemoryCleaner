#include "pch.h"
#include "MemoryCleaner.h"
#include "MainWindow.xaml.h" // 包含智能指针定义

static HANDLE openProcessForTrim(DWORD pid) {
    // 需要 PROCESS_SET_QUOTA 以调用 EmptyWorkingSet
    return OpenProcess(PROCESS_QUERY_INFORMATION |
                       PROCESS_SET_QUOTA |
                       PROCESS_VM_OPERATION |
                       PROCESS_VM_READ,
                       FALSE, pid);
}

FARPROC MemoryCleaner::resolveNtSetSystemInformation() {
    // 优先 GetModuleHandle，失败再 LoadLibrary
    HMODULE mod = GetModuleHandleW(L"ntdll.dll");
    if (!mod) mod = LoadLibraryW(L"ntdll.dll");
    if (!mod) return nullptr;
    return GetProcAddress(mod, "NtSetSystemInformation");
}

bool MemoryCleaner::cleanWorkingSet() {
    return EmptyWorkingSet(GetCurrentProcess()) != FALSE;
}

bool MemoryCleaner::cleanSystemWorkingSet() {
    // 使用智能指针管理进程快照句柄
    UniqueHandle snap(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), HandleDeleter{});
    if (!snap || snap.get() == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);
    if (!Process32First(snap.get(), &pe)) {
        return false;
    }

    do {
        if (pe.th32ProcessID == 0) continue; // System Idle
        
        // 使用智能指针管理进程句柄
        UniqueHandle h(openProcessForTrim(pe.th32ProcessID), HandleDeleter{});
        if (h) {
            // 忽略失败：非关键路径，有的进程权限不足
            EmptyWorkingSet(h.get());
        }
    } while (Process32Next(snap.get(), &pe));

    return true;
}


bool MemoryCleaner::cleanStandbyList(bool clearLowPriority) {
    using pNtSetSystemInformation = NTSTATUS(NTAPI*)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG);

    auto proc = reinterpret_cast<pNtSetSystemInformation>(resolveNtSetSystemInformation());
    if (!proc) return false;

    SYSTEM_MEMORY_LIST_COMMAND cmd = clearLowPriority ?
        MemoryPurgeLowPriorityStandbyList : MemoryPurgeStandbyList;

    NTSTATUS status = proc(SystemMemoryListInformation, &cmd, sizeof(cmd));
    return NT_SUCCESS(status);
}

bool MemoryCleaner::cleanModifiedPageList() {
    using pNtSetSystemInformation = NTSTATUS(NTAPI*)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG);
    auto proc = reinterpret_cast<pNtSetSystemInformation>(resolveNtSetSystemInformation());
    if (!proc) return false;

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryPurgeModifiedPageList;
    NTSTATUS status = proc(SystemMemoryListInformation, &cmd, sizeof(cmd));
    return NT_SUCCESS(status);
}

bool MemoryCleaner::cleanCombinePageList() {
    using pNtSetSystemInformation = NTSTATUS(NTAPI*)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG);
    auto proc = reinterpret_cast<pNtSetSystemInformation>(resolveNtSetSystemInformation());
    if (!proc) return false;

    SYSTEM_MEMORY_LIST_COMMAND cmd = MemoryPurgeCombinedPageList;
    NTSTATUS status = proc(SystemMemoryListInformation, &cmd, sizeof(cmd));
    return NT_SUCCESS(status);
}

bool MemoryCleaner::cleanAllProcessesWorkingSet() {
    // 为保持与你原函数一致，调用同样逻辑
    return cleanSystemWorkingSet();
}

