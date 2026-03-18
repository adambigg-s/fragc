#include "../vendor/minifb/include/MiniFB.h"
#include "hot_reload.h"
#include "shader.h"
#include <minwindef.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <winnt.h>

static const char *dll_paths[2] = {
    "./target/shader0.dll",
    "./target/shader1.dll",
};
static bool dll_flip = false;
static FragFn frag_fn = NULL;
static HMODULE dll_lib = NULL;
static FILETIME save_time = {};

i32 main(i32 argc, char *arvg[]) {
    bool success;

    Sampler start = {};
    success = sampler_load("res/art_museum.jpg", &start);
    if (!success) {
        exit(3);
    }

    Sampler end = {};
    success = sampler_clone(&start, &end);
    if (!success) {
        exit(3);
    }

    struct mfb_window *frame_buffer = mfb_open("FragC", end.width, end.height);
    if (!frame_buffer) {
        fprintf(stderr, "Error opening window");
        exit(3);
    }
    mfb_set_target_fps(60);

    hr_reload(dll_paths[(usize)dll_flip], &dll_lib, &frag_fn);

    char *file_name = "src/shaders/test.c";
    hr_get_time(file_name, &save_time);

    while (true) {
        if (mfb_get_key_buffer(frame_buffer)[KB_KEY_ESCAPE]) {
            break;
        }

        mfb_wait_sync(frame_buffer);

        bool updated = mfb_get_key_buffer(frame_buffer)[KB_KEY_R];

        if (hr_has_changed(file_name, &save_time)) {
            dll_flip = !dll_flip;
            if (hr_reload(dll_paths[dll_flip], &dll_lib, &frag_fn)) {
                updated = true;
            } else {
                break;
            }
        }

        if (updated && frag_fn) {
#pragma omp parallel for
            for (usize row = 0; row < end.height; row++) {
                for (usize col = 0; col < end.width; col++) {
                    Uniform uni = {.vp_height = end.height, .vp_width = end.width, .sampler_count = 1};
                    Vec2 uv = vec2(((f32)col + 0.5) / end.width, ((f32)row + 0.5) / end.height);
                    Vec4 color = (frag_fn)(&uni, &start, uv);
                    sampler_set(&end, col, row, color);
                }
            }
#pragma endregion

            if (mfb_update_ex(frame_buffer, end.data, end.width, end.height)) {
                updated = false;
                break;
            }
        } else {
            if (mfb_update_events(frame_buffer)) {
                break;
            }
        }
    }

    success = sampler_write("target/output.png", &end);
    if (!success) {
        exit(3);
    }

    sampler_free(start);
    sampler_free(end);
    fprintf(stdout, "Image processed\n");
    fflush(stdout);
    mfb_close(frame_buffer);
    return 0;
}
