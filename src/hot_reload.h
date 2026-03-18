#ifndef HOT_RELOAD_H
#define HOT_RELOAD_H

#include "shader.h"
#include <Windows.h>
#include <fileapi.h>
#include <handleapi.h>
#include <stdio.h>
#include <winnt.h>

// bool hr_load(FragFn *location, char *path) {
// }

typedef enum file_time_ordering {
    FtEqual,
    FtLess,
    FtGreater,
} FileTimeOrdering;

static bool hr_changed_time(char *file_name, FILETIME *filetime) {
    HANDLE handle = CreateFileA(
        file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!handle) {
        fprintf(stderr, "Error getting file handle %s\n", file_name);
        return false;
    }

    if (!GetFileTime(handle, NULL, NULL, filetime)) {
        fprintf(stderr, "Error retrieving last change time %s\n", file_name);
        CloseHandle(handle);
        return false;
    }

    CloseHandle(handle);
    return true;
}

static FileTimeOrdering hr_compare_times(FILETIME *rhs, FILETIME *lhs) {
    if (rhs->dwHighDateTime < lhs->dwHighDateTime) {
        return FtLess;
    } else if (rhs->dwHighDateTime > lhs->dwHighDateTime) {
        return FtGreater;
    } else {
        return FtEqual;
    }
}

#endif
