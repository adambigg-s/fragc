#include "../vendor/minifb/include/MiniFB.h"
#include "hot_reload.h"
#include "shader.h"
#include <minwindef.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <winnt.h>

static const usize DEFAULT_WIDTH = 800;
static const usize DEFAULT_HEIGHT = 600;
static const usize BUFFER_COUNT = 8;
static const char *DEFAULT_SHADER = "./src/shaders/main.c";
static const char *DEFAULT_TARGET = "./target/output.png";
static const char *DLIB_PATH[2] = {
    "./target/shader0.dll",
    "./target/shader1.dll",
};
static bool dlib_flip = true;
static HMODULE dlib = NULL;
static FragFn frag_fn = NULL;
static FILETIME file_alter = {};

i32 main(i32 argc, char *argv[]) {
    bool success;
    Sampler *samplers[BUFFER_COUNT];

    const char *output_path = NULL;
    if (argc > 1) {
        output_path = argv[1];
    }

    Sampler start = {};
    if (output_path) {
        success = sampler_load(output_path, &start);
        if (!success) {
            fprintf(stderr, "Defaulting to standard sampler\n");
            sampler_new(&start, DEFAULT_WIDTH, DEFAULT_HEIGHT);
        }
    } else {
        sampler_new(&start, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    }

    Sampler end = {};
    success = sampler_clone(&start, &end);
    if (!success) {
        exit(3);
    }

    struct mfb_window *frame_buffer = mfb_open("fragc Preview Window", end.width, end.height);
    if (!frame_buffer) {
        fprintf(stderr, "Error opening window");
        exit(3);
    }
    mfb_set_target_fps(60);

    const char *shader_path = NULL;
    if (argc > 2) {
        shader_path = argv[2];
    } else {
        shader_path = DEFAULT_SHADER;
    }
    hr_get_time(shader_path, &file_alter);

    bool render = true;
    while (mfb_wait_sync(frame_buffer)) {
        const u8 *keys = mfb_get_key_buffer(frame_buffer);
        const u8 *mouse = mfb_get_mouse_button_buffer(frame_buffer);
        if (keys[KB_KEY_ESCAPE]) {
            break;
        }
        if (keys[KB_KEY_R]) {
            render = true;
        }

        if (hr_has_changed(shader_path, &file_alter) || render) {
            dlib_flip = !dlib_flip;
            if (hr_reload(DLIB_PATH[dlib_flip], shader_path, &dlib, &frag_fn)) {
                render = true;
            }
        }

        if (render && frag_fn) {
#pragma region
#pragma omp parallel for
            for (usize row = 0; row < end.height; row++) {
                for (usize col = 0; col < end.width; col++) {
                    Uniform uni = {.vp_height = end.height, .vp_width = end.width, .sampler_count = 1};
                    if (!mouse[MOUSE_LEFT]) {
                        uni.mouse_pos.x = mfb_get_mouse_x(frame_buffer);
                        uni.mouse_pos.y = mfb_get_mouse_y(frame_buffer);
                    } else {
                        uni.mouse_pos.z = mfb_get_mouse_x(frame_buffer);
                        uni.mouse_pos.w = mfb_get_mouse_y(frame_buffer);
                    }
                    Vec2 uv = vec2(((f32)col + 0.5) / end.width, ((f32)row + 0.5) / end.height);
                    Vec4 color = (frag_fn)(&uni, &start, uv);
                    sampler_set(&end, col, row, color);
                }
            }
#pragma endregion

            mfb_update_ex(frame_buffer, end.data, end.width, end.height);
            render = false;
        } else {
            mfb_update_events(frame_buffer);
        }
    }

    success = sampler_write(DEFAULT_TARGET, &end);
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
