#include "../vendor/minifb/include/MiniFB.h"
#include "hot_reload.h"
#include "shader.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

// static FragFn frag_fn = NULL;

int main() {
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

    char *file_name = "src/shaders/test.c";
    FILETIME last_time = {0};
    hr_changed_time(file_name, &last_time);

    while (true) {
        if (mfb_get_key_buffer(frame_buffer)[KB_KEY_ESCAPE]) {
            break;
        }

        FILETIME new_time = {0};
        hr_changed_time(file_name, &new_time);

        mfb_wait_sync(frame_buffer);
        bool updated = false;
        if (mfb_get_key_buffer(frame_buffer)[KB_KEY_R]) {
            updated = true;
        }

        if (hr_compare_times(&last_time, &new_time) != FtEqual) {
            updated = true;
        }
        last_time = new_time;
        if (updated) {
#pragma omp parallel for
            for (usize row = 0; row < end.height; row++) {
                for (usize col = 0; col < end.width; col++) {
                    Uniform uni = {.vp_height = end.height, .vp_width = end.width, .sampler_count = 1};
                    Vec2 uv = vec2(((f32)col + 0.5) / end.width, ((f32)row + 0.5) / end.height);
                    Vec4 color = frag(&uni, &start, uv);
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
