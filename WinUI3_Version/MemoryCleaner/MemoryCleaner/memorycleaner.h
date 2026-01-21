#pragma once

#include <windows.h>
#include <winternl.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdint>
#include "pch.h"

// 某些编译环境需要显式链接 Psapi.lib
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "ntdll.lib")

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

// 如果 SystemMemoryListInformation 未定义，则手动定义
#ifndef SystemMemoryListInformation
#define SystemMemoryListInformation (SYSTEM_INFORMATION_CLASS)0x50
#endif

// 与 ntdll 的 SystemMemoryListInformation 一起使用的命令枚举
typedef enum _SYSTEM_MEMORY_LIST_COMMAND {
    MemoryPurgeStandbyList = 1,
    MemoryPurgeLowPriorityStandbyList,
    MemoryPurgeModifiedPageList,
    MemoryPurgeCombinedPageList
} SYSTEM_MEMORY_LIST_COMMAND;

class MemoryCleaner {
public:
    // 清理当前进程工作集
    static bool cleanWorkingSet();

    // 遍历系统进程并尝试清理各自工作集（需要足够权限）
    static bool cleanSystemWorkingSet();

    // 清理待机列表；clearLowPriority=true 仅清理低优先级待机列表
    static bool cleanStandbyList(bool clearLowPriority);

    // 清理"已修改页列表"
    static bool cleanModifiedPageList();

    // 清理"合并页列表"
    static bool cleanCombinePageList();

    // 同 cleanSystemWorkingSet（保持原函数名以兼容）
    static bool cleanAllProcessesWorkingSet();

private:
    // 动态解析 ntdll!NtSetSystemInformation，避免显式依赖
    static FARPROC resolveNtSetSystemInformation();
};