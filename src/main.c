#include "shader.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

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

#pragma omp parallel for
    for (usize row = 0; row < end.height; row++) {
        for (usize col = 0; col < end.width; col++) {
            Uniform uni = {.height = end.height, .width = end.width, .sampler_count = 1};
            Vec2 uv = vec2(((f32)col + 0.5) / end.width, ((f32)row + 0.5) / end.height);
            Vec4 color = frag(&uni, &start, uv);
            sampler_set(&end, col, row, color);
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
    return 0;
}
