# MemoryCleaner Documentation

## Overview

The `MemoryCleaner` class provides a comprehensive set of functions for memory management and optimization on Windows systems. It implements various memory cleaning techniques to improve system performance by freeing different types of memory caches and working sets.

## Function Reference

### `cleanWorkingSet()`

**Description**: Cleans the working set of the current process.

**Function Signature**:
```cpp
static bool cleanWorkingSet();
```

**Parameters**: None

**Return Value**:
- `true` if the operation was successful
- `false` if the operation failed

**Implementation Details**:
- Uses the Windows API function `EmptyWorkingSet()`
- Targets only the current process
- Reduces the physical memory usage of the calling application

**Usage Example**:
```cpp
if (MemoryCleaner::cleanWorkingSet()) {
    qDebug() << "Current process working set cleaned successfully";
}
```

---

### `cleanSystemWorkingSet()`

**Description**: Cleans the working sets of all running processes in the system (excluding system processes).

**Function Signature**:
```cpp
static bool cleanSystemWorkingSet();
```

**Parameters**: None

**Return Value**:
- `true` if the operation was successful
- `false` if any part of the operation failed

**Implementation Details**:
- Uses `CreateToolhelp32Snapshot()` to enumerate all processes
- Iterates through each process and calls `EmptyWorkingSet()`
- Skips process ID 0 (System Idle Process)
- Requires `PROCESS_QUERY_INFORMATION`, `PROCESS_SET_QUOTA`, `PROCESS_VM_OPERATION`, and `PROCESS_VM_READ` access rights

**Usage Example**:
```cpp
if (MemoryCleaner::cleanSystemWorkingSet()) {
    qDebug() << "System working sets cleaned successfully";
}
```

---

### `cleanStandbyList(bool clearLowPriority)`

**Description**: Cleans the system standby list, which contains memory pages that are no longer actively used but are kept in RAM for potential future use.

**Function Signature**:
```cpp
static bool cleanStandbyList(bool clearLowPriority);
```

**Parameters**:
- `clearLowPriority`: Boolean flag indicating whether to clear only low-priority standby list
  - `true`: Clear low-priority standby list only
  - `false`: Clear all standby lists

**Return Value**:
- `true` if the operation was successful
- `false` if the function is not available or operation failed

**Implementation Details**:
- Uses the undocumented Windows API function `NtSetSystemInformation()` from ntdll.dll
- Supports two modes: general standby list cleanup and low-priority standby list cleanup
- Uses `SYSTEM_MEMORY_LIST_COMMAND` enumeration with appropriate command values

**Usage Example**:
```cpp
// Clear all standby lists
if (MemoryCleaner::cleanStandbyList(false)) {
    qDebug() << "Standby list cleaned successfully";
}

// Clear only low-priority standby list
if (MemoryCleaner::cleanStandbyList(true)) {
    qDebug() << "Low-priority standby list cleaned successfully";
}
```

---

### `cleanModifiedPageList()`

**Description**: Cleans the modified page list, which contains memory pages that have been modified but not yet written to disk.

**Function Signature**:
```cpp
static bool cleanModifiedPageList();
```

**Parameters**: None

**Return Value**:
- `true` if the operation was successful
- `false` if the function is not available or operation failed

**Implementation Details**:
- Uses `NtSetSystemInformation()` with `MemoryPurgeModifiedPageList` command
- Forces modified pages to be written to disk and removed from RAM
- Helps free memory occupied by dirty pages waiting to be flushed

**Usage Example**:
```cpp
if (MemoryCleaner::cleanModifiedPageList()) {
    qDebug() << "Modified page list cleaned successfully";
}
```

---

### `cleanCombinePageList()`

**Description**: Cleans the combined page list, which includes multiple types of memory pages.

**Function Signature**:
```cpp
static bool cleanCombinePageList();
```

**Parameters**: None

**Return Value**:
- `true` if the operation was successful
- `false` if the function is not available or operation failed

**Implementation Details**:
- Uses `NtSetSystemInformation()` with `MemoryPurgeCombinedPageList` command
- Cleans a combination of different memory page types
- Provides comprehensive memory cleanup

**Usage Example**:
```cpp
if (MemoryCleaner::cleanCombinePageList()) {
    qDebug() << "Combined page list cleaned successfully";
}
```

---

### `cleanAllProcessesWorkingSet()`

**Description**: Cleans the working sets of all processes in the system (similar to `cleanSystemWorkingSet()` but with a different implementation approach).

**Function Signature**:
```cpp
static bool cleanAllProcessesWorkingSet();
```

**Parameters**: None

**Return Value**:
- `true` if the operation was successful
- `false` if any part of the operation failed

**Implementation Details**:
- Uses the same approach as `cleanSystemWorkingSet()` but implemented separately
- Enumerates all processes using `CreateToolhelp32Snapshot()`
- Applies `EmptyWorkingSet()` to each valid process
- Skips process ID 0

**Usage Example**:
```cpp
if (MemoryCleaner::cleanAllProcessesWorkingSet()) {
    qDebug() << "All processes working sets cleaned successfully";
}
```

---

### `getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS)`

**Description**: Retrieves information about the system's working set memory usage.

**Function Signature**:
```cpp
static bool getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS);
```

**Parameters**:
- `totalWS`: Reference to store the total working set size (output parameter)
- `sharedWS`: Reference to store the shared working set size (output parameter)
- `privateWS`: Reference to store the private working set size (output parameter)

**Return Value**:
- `true` if the information was successfully retrieved
- `false` if the operation failed

**Implementation Details**:
- Enumerates all processes and sums their working set sizes
- Uses `GetProcessMemoryInfo()` to get memory statistics for each process
- Calculates shared working set as 30% of total working set (simplified estimation)
- Calculates private working set as total minus shared

**Usage Example**:
```cpp
quint64 total, shared, private;
if (MemoryCleaner::getSystemWorkingSetInfo(total, shared, private)) {
    qDebug() << "Total WS:" << total << "bytes";
    qDebug() << "Shared WS:" << shared << "bytes";
    qDebug() << "Private WS:" << private << "bytes";
}
```

## Memory Management Concepts

### Working Set
- The set of memory pages currently resident in physical memory for a process
- Cleaning the working set forces pages to be paged out to disk
- Can improve system responsiveness but may cause temporary performance degradation

### Standby List
- Contains memory pages that are no longer actively used but kept in RAM
- Pages can be quickly reused if needed
- Cleaning the standby list frees this memory for other applications

### Modified Page List
- Contains pages that have been modified but not yet written to disk
- Cleaning forces these pages to be written to disk (flushed)
- Important for data consistency and memory reclamation

## Dependencies

- **Windows API**: Uses various Windows memory management functions
- **Qt**: Uses `QtGlobal` for `quint64` type
- **ntdll.dll**: For undocumented system memory management functions

## Notes

- Some functions use undocumented Windows APIs that may change in future versions
- Memory cleaning operations should be used judiciously as they can impact performance
- The shared working set calculation is an estimation (30% of total)
- Process enumeration skips system processes (PID 0) for safety

## Error Handling

All functions return boolean values indicating success or failure. It's recommended to check return values and handle errors appropriately in calling code.# MemoryCleaner Class Documentation
## Overview
The MemoryCleaner class provides static methods for managing Windows memory optimization operations. It implements various memory cleaning techniques including working set trimming, standby list purging, and system-wide memory management.

## Function Documentation
### cleanWorkingSet()
Description : Trims the working set of the current process by removing pages that are not actively being used.

Parameters : None

Returns : bool - true if successful, false if failed

Implementation Details :

- Uses Windows API EmptyWorkingSet() function
- Targets only the current process
- Safe operation that doesn't affect application stability
Usage Example :

```
if (MemoryCleaner::cleanWorkingSet()) {
    // Working set successfully trimmed
}
```
### cleanSystemWorkingSet()
Description : Trims the working sets of all running processes on the system (excluding system processes).

Parameters : None

Returns : bool - true if successful, false if failed

Implementation Details :

- Iterates through all processes using CreateToolhelp32Snapshot()
- Skips process ID 0 (System Idle Process)
- Requires PROCESS_QUERY_INFORMATION , PROCESS_SET_QUOTA , PROCESS_VM_OPERATION , and PROCESS_VM_READ access rights
- Calls EmptyWorkingSet() on each valid process
Usage Example :
```
if (MemoryCleaner::cleanSystemWorkingSet()) {
    // System working sets successfully trimmed
}
```

### cleanStandbyList(bool clearLowPriority)
Description : Purges the Windows standby list, which contains memory pages that are cached but not actively in use.

Parameters :

- clearLowPriority ( bool ): If true , clears only low-priority standby pages; if false , clears all standby pages
Returns : bool - true if successful, false if failed or function not available

Implementation Details :

- Uses undocumented Windows API NtSetSystemInformation() via ntdll.dll
- Supports selective clearing of low-priority standby pages
- Uses MemoryPurgeStandbyList or MemoryPurgeLowPriorityStandbyList commands
Usage Example :
