#ifndef HOT_RELOAD_H
#define HOT_RELOAD_H

#include "shader.h"
#include <Windows.h>
#include <fileapi.h>
#include <handleapi.h>
#include <libloaderapi.h>
#include <minwindef.h>
#include <stdio.h>
#include <winnt.h>

typedef enum file_time_ordering_t {
    FtEqual,
    FtLess,
    FtGreater,
} FileTimeOrdering;

static bool hr_get_time(const char *file_name, FILETIME *filetime) {
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

static FileTimeOrdering hr_compare_times(const FILETIME *rhs, const FILETIME *lhs) {
    if (rhs->dwHighDateTime < lhs->dwHighDateTime) {
        return FtLess;
    } else if (rhs->dwHighDateTime > lhs->dwHighDateTime) {
        return FtGreater;
    } else {
        return FtEqual;
    }
}

static bool hr_has_changed(const char *file_name, FILETIME *filetime) {
    FILETIME curr_time;
    hr_get_time(file_name, &curr_time);
    FileTimeOrdering comparison = hr_compare_times(filetime, &curr_time);
    *filetime = curr_time;
    return comparison != FtEqual;
}

static bool hr_reload(const char *out_dll_path, HMODULE *old_lib, FragFn *out_frag) {
    char cmd_buffer[1024];
    snprintf(cmd_buffer, sizeof(cmd_buffer),
        "clang -O3 -shared "
        "-o %s "
        "./src/shaders/test.c ./src/math_impl.c ./src/shader_impl.c",
        out_dll_path);
    fprintf(stdout, "Hot-Reload: %s\n", cmd_buffer);
    if (!system(cmd_buffer)) {
        fprintf(stderr, "Shader compilation failed\n");
    }

    HMODULE lib = LoadLibrary(out_dll_path);
    if (!lib) {
        fprintf(stderr, "LoadLibrary failed\n");
    }

    FragFn fn = (FragFn)GetProcAddress(lib, "frag");
    if (!fn) {
        fprintf(stderr, "GetProcAddress failed\n");
        FreeLibrary(lib);
        return false;
    }

    if (*old_lib) {
        FreeLibrary(*old_lib);
    }
    *old_lib = lib;
    *out_frag = fn;
    fprintf(stdout, "Hot-Reload loaded %s\n", out_dll_path);

    fflush(stdout);
    return true;
}

#endif
