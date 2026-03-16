#ifndef SHADER_H
#define SHADER_H

#include "../vendor/stb/stb_image.h"
#include "../vendor/stb/stb_image_write.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

f32 minx(f32 a, f32 b);

f32 maxx(f32 a, f32 b);

f32 clampx(f32 x, f32 min, f32 max);

f32 signx(f32 x);

f32 floorx(f32 x);

f32 ceilx(f32 x);

f32 fractx(f32 x);

f32 mixx(f32 a, f32 b, f32 mix);

f32 lerpx(f32 a, f32 b, f32 t);

typedef struct vec2_t {
    f32 x;
    f32 y;
} Vec2;

Vec2 vec2(f32 x, f32 y);

Vec2 vec2_neg(Vec2 v);

Vec2 vec2_add(Vec2 a, Vec2 b);

Vec2 vec2_sub(Vec2 a, Vec2 b);

Vec2 vec2_mul(Vec2 a, f32 b);

Vec2 vec2_div(Vec2 a, f32 b);

f32 vec2_dot(Vec2 a, Vec2 b);

f32 vec2_mag(Vec2 v);

Vec2 vec2_normalize(Vec2 v);

typedef struct vec3_t {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

Vec3 vec3(f32 x, f32 y, f32 z);

Vec3 vec3_neg(Vec3 v);

Vec3 vec3_add(Vec3 a, Vec3 b);

Vec3 vec3_sub(Vec3 a, Vec3 b);

Vec3 vec3_mul(Vec3 a, f32 b);

Vec3 vec3_div(Vec3 a, f32 b);

f32 vec3_dot(Vec3 a, Vec3 b);

f32 vec3_mag(Vec3 v);

Vec3 vec3_normalize(Vec3 v);

typedef struct vec4_t {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Vec4;

Vec4 vec4(f32 x, f32 y, f32 z, f32 w);

Vec4 vec4_neg(Vec4 v);

Vec4 vec4_add(Vec4 a, Vec4 b);

Vec4 vec4_sub(Vec4 a, Vec4 b);

Vec4 vec4_mul(Vec4 a, f32 b);

Vec4 vec4_div(Vec4 a, f32 b);

f32 vec4_dot(Vec4 a, Vec4 b);

f32 vec4_mag(Vec4 v);

Vec4 vec4_normalize(Vec4 v);

typedef struct uniform_t {
    f32 time;
    i32 width;
    i32 height;
    usize sampler_count;
} Uniform;

typedef enum sample_method_t {
    Nearest,
    Bilinear,
    Trilinear,
} SamplerMethod;

typedef struct sampler_t {
    u8 *data;
    usize width;
    usize height;
} Sampler;

Vec4 sample(const Sampler *sampler, SamplerMethod method, f32 x, f32 y);

bool sampler_new(Sampler *sampler, usize width, usize height);

u8 *sampler_get(Sampler *sampler, usize x, usize y);

bool sampler_clone(const Sampler *from, Sampler *target);

bool sampler_load(char *path, Sampler *sampler);

bool sampler_write(char *path, Sampler *sampler);

void sampler_free(Sampler sampler);

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv);

#endif
