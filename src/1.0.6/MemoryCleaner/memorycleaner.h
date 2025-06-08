// memorycleaner.h
#ifndef MEMORYCLEANER_H
#define MEMORYCLEANER_H

#include <QtGlobal>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <winternl.h>

typedef enum _SYSTEM_MEMORY_LIST_COMMAND {
    MemoryPurgeStandbyList = 1,
    MemoryPurgeLowPriorityStandbyList,
    MemoryPurgeModifiedPageList,
    MemoryPurgeCombinedPageList
} SYSTEM_MEMORY_LIST_COMMAND;

#define SystemMemoryListInformation 80


class MemoryCleaner
{
public:
    static bool cleanWorkingSet();
    static bool cleanSystemWorkingSet();
    static bool cleanStandbyList(bool clearLowPriority);
    static bool cleanModifiedPageList();
    static bool cleanCombinePageList();
    static bool cleanAllProcessesWorkingSet();
    static bool getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS);
};

#endif // MEMORYCLEANER_H
