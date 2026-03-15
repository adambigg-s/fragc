#ifndef SHADER_H
#define SHADER_H

#include "../vendor/stb/stb_image.h"
#include "../vendor/stb/stb_image_write.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uintptr_t usize;
typedef intptr_t isize;

typedef struct vec2_t {
    f32 x;
    f32 y;
} Vec2;

inline Vec2 vec2(f32 x, f32 y) {
    return (Vec2){.x = x, .y = y};
}

typedef struct vec3_t {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

inline Vec3 vec3(f32 x, f32 y, f32 z) {
    return (Vec3){.x = x, .y = y, .z = z};
}

typedef struct vec4_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

inline Vec4 vec4(f32 x, f32 y, f32 z, f32 w) {
    return (Vec4){.x = x, .y = y, .z = z, .w = w};
}

typedef struct uniform_t {

} Uniform;

typedef struct sampler_t {
    u8 *data;
    usize width;
    usize height;
} Sampler;

inline Vec4 sampler_sample(Sampler *sampler, f32 x, f32 y) {
    isize sx = x * (sampler->width - 1);
    isize sy = y * (sampler->height - 1);
    f32 r = sampler->data[(sampler->width * sy + sx) * 3 + 0] / 255.0;
    f32 g = sampler->data[(sampler->width * sy + sx) * 3 + 1] / 255.0;
    f32 b = sampler->data[(sampler->width * sy + sx) * 3 + 2] / 255.0;
    return vec4(r, g, b, 1.0);
}

inline u8 *sampler_get(Sampler *sampler, usize x, usize y) {
    return &sampler->data[(sampler->width * y + x) * 3];
}

inline bool sampler_load(char *path, Sampler *sampler) {
    i32 width, height, levels;
    u8 *data = stbi_load(path, &width, &height, &levels, 3);
    if (!data) {
        fprintf(stderr, "Error loading sampler: %s\n", path);
        return false;
    }
    sampler->data = data;
    sampler->height = height;
    sampler->width = width;
    return true;
}

inline bool sampler_write(char *path, Sampler *sampler) {
    i32 success = stbi_write_png(path, sampler->width, sampler->height, 3, sampler->data, sampler->width * 3);
    if (!success) {
        fprintf(stderr, "Error writing sampler: %s\n", path);
        return false;
    }
    return true;
}

inline void sampler_free(Sampler sampler) {
    stbi_image_free(sampler.data);
}

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv);

#endif
