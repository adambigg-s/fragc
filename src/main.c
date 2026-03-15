#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

int main() {
    Sampler image = {};
    bool success;
    success = sampler_load("res/art_museum.jpg", &image);
    if (!success) {
        exit(3);
    }

    for (usize row = 0; row < image.height; row++) {
        for (usize col = 0; col < image.width; col++) {
            Uniform uni = {};
            Vec2 uv = vec2((f32)col / image.width, (f32)row / image.height);
            Vec4 out = frag(&uni, &image, uv);

            u8 *color = sampler_get(&image, col, row);

            *(color + 0) = out.x * 255.0;
            *(color + 1) = out.y * 255.0;
            *(color + 2) = out.z * 255.0;
        }
    }

    success = sampler_write("target/output.png", &image);
    if (!success) {
        exit(3);
    }

    sampler_free(image);
    printf("Image processed");
    return 0;
}
