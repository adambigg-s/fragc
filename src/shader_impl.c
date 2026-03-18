#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

Vec4 sampler_sample_nearest(const Sampler *sampler, f32 x, f32 y);

Vec4 sampler_sample_bilinear(const Sampler *sampler, f32 x, f32 y);

Vec4 sampler_sample_bicubic(const Sampler *sampler, f32 x, f32 y);

u8 *sampler_get(Sampler *sampler, usize x, usize y);

Vec4 sampler_get_slice(const Sampler *sampler, usize x, usize y);

Vec4 sampler_get_color(const Sampler *sampler, usize x, usize y);

void sampler_rgba_argb_flip(Sampler *sampler);

Vec4 sample(const Sampler *sampler, SamplerMethod method, f32 x, f32 y) {
    switch (method) {
    case Nearest:
        return sampler_sample_nearest(sampler, x, y);
        break;
    case Bilinear:
        return sampler_sample_bilinear(sampler, x, y);
        break;
    case Bicubic:
        return sampler_sample_bicubic(sampler, x, y);
        break;
    }
}

bool sampler_new(Sampler *sampler, usize width, usize height) {
    *sampler = (Sampler){.data = (u8 *)malloc(height * width * 4), .width = width, .height = height};
    if (!sampler->data) {
        fprintf(stderr, "Error allocating for sampler\n");
        return false;
    }
    return true;
}

void sampler_set(Sampler *sampler, usize x, usize y, Vec4 data) {
    u8 *color = sampler_get(sampler, x, y);
    color[0] = clampx(data.z * 255.9999, 0.0, 255);
    color[1] = clampx(data.y * 255.9999, 0.0, 255);
    color[2] = clampx(data.x * 255.9999, 0.0, 255);
    color[3] = clampx(data.w * 255.9999, 0.0, 255);
}

Vec4 sampler_sample_nearest(const Sampler *sampler, f32 x, f32 y) {
    isize sx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    isize sy = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    Vec4 color = sampler_get_color(sampler, sx, sy);
    return vec4(color.x, color.y, color.z, 1.0);
}

Vec4 sampler_sample_bilinear(const Sampler *sampler, f32 x, f32 y) {
    f32 tx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    f32 ty = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    f32 fx = fractx(tx);
    f32 fy = fractx(ty);
    isize sx = tx;
    isize sy = ty;

    Vec4 v00 = sampler_get_color(sampler, sx, sy);
    Vec4 v01 = sampler_get_color(sampler, sx + 1, sy);
    Vec4 v10 = sampler_get_color(sampler, sx, sy + 1);
    Vec4 v11 = sampler_get_color(sampler, sx + 1, sy + 1);
    f32 w00 = (1.0 - fx) * (1.0 - fy);
    f32 w01 = fx * (1.0 - fy);
    f32 w10 = (1.0 - fx) * fy;
    f32 w11 = fx * fy;

    // clang-format off
    return
        vec4_add(
        vec4_add(
        vec4_add(
            vec4_mul(v00, w00),
            vec4_mul(v01, w01)),
            vec4_mul(v10, w10)),
            vec4_mul(v11, w11));
    // clang-format on
}

Vec4 sampler_sample_bicubic(const Sampler *sampler, f32 x, f32 y) {
    return vec4(0.0, 0.0, 0.0, 0.0);
}

bool sampler_clone(const Sampler *from, Sampler *target) {
    return sampler_new(target, from->width, from->height);
}

bool sampler_load(const char *path, Sampler *sampler) {
    i32 width, height, levels;
    u8 *data = stbi_load(path, &width, &height, &levels, 4);
    if (!data) {
        fprintf(stderr, "Error loading sampler: %s\n", path);
        return false;
    }
    sampler->data = data;
    sampler->height = height;
    sampler->width = width;
    sampler_rgba_argb_flip(sampler);
    return true;
}

bool sampler_write(const char *path, Sampler *sampler) {
    sampler_rgba_argb_flip(sampler);
    i32 success = stbi_write_png(path, sampler->width, sampler->height, 4, sampler->data, sampler->width * 4);
    if (!success) {
        fprintf(stderr, "Error writing sampler: %s\n", path);
        return false;
    }
    return true;
}

void sampler_free(Sampler sampler) {
    stbi_image_free(sampler.data);
}

u8 *sampler_get(Sampler *sampler, usize x, usize y) {
    return &sampler->data[(sampler->width * y + x) * 4];
}

Vec4 sampler_get_slice(const Sampler *sampler, usize x, usize y) {
    u8 *ptr = &sampler->data[(sampler->width * y + x) * 4];
    return vec4_div(vec4(ptr[0], ptr[1], ptr[2], ptr[3]), 255.0);
}

Vec4 sampler_get_color(const Sampler *sampler, usize x, usize y) {
    u8 *ptr = &sampler->data[(sampler->width * y + x) * 4];
    return vec4_div(vec4(ptr[2], ptr[1], ptr[0], ptr[3]), 255.0);
}

void sampler_rgba_argb_flip(Sampler *sampler) {
    for (usize i = 0; i < sampler->height; i++) {
        for (usize j = 0; j < sampler->width; j++) {
            Vec4 old = sampler_get_slice(sampler, j, i);
            sampler_set(sampler, j, i, old);
        }
    }
}
