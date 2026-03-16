#ifndef SHADER_H
#define SHADER_H

#include "../vendor/stb/stb_image.h"
#include "../vendor/stb/stb_image_write.h"
#include <assert.h>
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

static inline Vec2 vec2(f32 x, f32 y) {
    return (Vec2){.x = x, .y = y};
}

static inline Vec2 vec2_neg(Vec2 v) {
    return (Vec2){.x = -v.x, .y = -v.y};
}

static inline Vec2 vec2_add(Vec2 a, Vec2 b) {
    return (Vec2){.x = a.x + b.x, .y = a.y + b.y};
}

static inline Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return (Vec2){.x = a.x - b.x, .y = a.y - b.y};
}

static inline Vec2 vec2_mul(Vec2 a, f32 b) {
    return (Vec2){.x = a.x * b, .y = a.y * b};
}

static inline Vec2 vec2_div(Vec2 a, f32 b) {
    return (Vec2){.x = a.x / b, .y = a.y / b};
}

typedef struct vec3_t {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

static inline Vec3 vec3(f32 x, f32 y, f32 z) {
    return (Vec3){.x = x, .y = y, .z = z};
}

static inline Vec3 vec3_neg(Vec3 v) {
    return (Vec3){.x = -v.x, .y = -v.y, .z = -v.z};
}

static inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

static inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
}

static inline Vec3 vec3_mul(Vec3 a, f32 b) {
    return (Vec3){.x = a.x * b, .y = a.y * b, .z = a.z * b};
}

static inline Vec3 vec3_div(Vec3 a, f32 b) {
    return (Vec3){.x = a.x / b, .y = a.y / b, .z = a.z / b};
}

typedef struct vec4_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

static inline Vec4 vec4(f32 x, f32 y, f32 z, f32 w) {
    return (Vec4){.x = x, .y = y, .z = z, .w = w};
}

static inline Vec4 vec4_neg(Vec4 v) {
    return (Vec4){.x = -v.x, .y = -v.y, .z = -v.z, .w = -v.w};
}

static inline Vec4 vec4_add(Vec4 a, Vec4 b) {
    return (Vec4){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w};
}

static inline Vec4 vec4_sub(Vec4 a, Vec4 b) {
    return (Vec4){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w};
}

static inline Vec4 vec4_mul(Vec4 a, f32 b) {
    return (Vec4){.x = a.x * b, .y = a.y * b, .z = a.z * b, .w = a.w * b};
}

static inline Vec4 vec4_div(Vec4 a, f32 b) {
    return (Vec4){.x = a.x / b, .y = a.y / b, .z = a.z / b, .w = a.w / b};
}

typedef struct uniform_t {

} Uniform;

typedef struct sampler_t {
    u8 *data;
    usize width;
    usize height;
} Sampler;

static inline Vec4 sampler_sample(const Sampler *sampler, f32 x, f32 y) {
    isize sx = x * (sampler->width - 1);
    isize sy = y * (sampler->height - 1);
    assert(sx >= 0 && sx < (isize)sampler->width);
    assert(sy >= 0 && sy < (isize)sampler->height);
    f32 r = sampler->data[(sampler->width * sy + sx) * 3 + 0] / 255.0;
    f32 g = sampler->data[(sampler->width * sy + sx) * 3 + 1] / 255.0;
    f32 b = sampler->data[(sampler->width * sy + sx) * 3 + 2] / 255.0;
    return vec4(r, g, b, 1.0);
}

static inline u8 *sampler_get(Sampler *sampler, usize x, usize y) {
    return &sampler->data[(sampler->width * y + x) * 3];
}

static inline Sampler sampler_clone(Sampler *sampler) {
    Sampler clone = {.data = (u8 *)malloc(sampler->height * sampler->width * 3),
        .width = sampler->width,
        .height = sampler->height};
    return clone;
}

static inline bool sampler_load(char *path, Sampler *sampler) {
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

static inline bool sampler_write(char *path, Sampler *sampler) {
    i32 success = stbi_write_png(path, sampler->width, sampler->height, 3, sampler->data, sampler->width * 3);
    if (!success) {
        fprintf(stderr, "Error writing sampler: %s\n", path);
        return false;
    }
    return true;
}

static inline void sampler_free(Sampler sampler) {
    stbi_image_free(sampler.data);
}

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv);

#endif
