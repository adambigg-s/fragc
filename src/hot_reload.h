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
    HANDLE handle = CreateFile(
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
    if (rhs->dwHighDateTime != lhs->dwHighDateTime) {
        return rhs->dwHighDateTime < lhs->dwHighDateTime ? FtLess : FtGreater;
    } else if (rhs->dwLowDateTime != lhs->dwLowDateTime) {
        return rhs->dwLowDateTime < lhs->dwLowDateTime ? FtLess : FtGreater;
    } else {
        return FtEqual;
    }
}

static bool hr_has_changed(const char *file_name, FILETIME *filetime) {
    FILETIME curr_time;
    if (!hr_get_time(file_name, &curr_time)) {
        return false;
    }
    FileTimeOrdering comparison = hr_compare_times(filetime, &curr_time);
    *filetime = curr_time;
    return comparison != FtEqual;
}

static bool hr_reload(const char *dlib_path, const char *shader_path, HMODULE *dlib, FragFn *frag) {
    char cmd_buffer[1024];
    snprintf(cmd_buffer, sizeof(cmd_buffer),
        "clang -O2 -shared "
        "-o %s "
        "%s ./src/math_impl.c ./src/shader_impl.c"
        "\n",
        dlib_path, shader_path);
    fprintf(stdout, "Hot-Reload: %s\n", cmd_buffer);
    if (system(cmd_buffer)) {
        fprintf(stderr, "Shader compilation failed\n");
        return false;
    }

    HMODULE new_dlib = LoadLibrary(dlib_path);
    if (!new_dlib) {
        fprintf(stderr, "LoadLibrary failed\n");
        return false;
    }

    FragFn frag_fn = (FragFn)GetProcAddress(new_dlib, "frag");
    if (!frag_fn) {
        fprintf(stderr, "GetProcAddress failed\n");
        FreeLibrary(new_dlib);
        return false;
    }

    FreeLibrary(*dlib);
    *dlib = new_dlib;
    *frag = frag_fn;

    fprintf(stdout, "Hot-Reload loaded %s\n", dlib_path);
    fflush(stdout);
    return true;
}

#endif
