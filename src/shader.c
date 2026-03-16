#include "shader.h"

f32 minx(f32 a, f32 b) {
    return a < b ? a : b;
}

f32 maxx(f32 a, f32 b) {
    return a > b ? a : b;
}

f32 clampx(f32 x, f32 min, f32 max) {
    x < min ? x = min : x;
    x > max ? x = max : x;
    return x;
}

f32 signx(f32 x) {
    return (x > 0) - (x < 0);
}

f32 floorx(f32 x) {
    return (f32)(i32)x;
}

f32 ceilx(f32 x) {
    return (f32)(i32)(x + 1.0);
}

f32 fractx(f32 x) {
    return x - (f32)(i32)x;
}

f32 mixx(f32 a, f32 b, f32 mix) {
    return mix * a + (1.0 - mix) * b;
}

f32 lerpx(f32 a, f32 b, f32 t) {
    return (b - a) * t + a;
}

Vec2 vec2(f32 x, f32 y) {
    return (Vec2){.x = x, .y = y};
}

Vec2 vec2_neg(Vec2 v) {
    return (Vec2){.x = -v.x, .y = -v.y};
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
    return (Vec2){.x = a.x + b.x, .y = a.y + b.y};
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return (Vec2){.x = a.x - b.x, .y = a.y - b.y};
}

Vec2 vec2_mul(Vec2 a, f32 b) {
    return (Vec2){.x = a.x * b, .y = a.y * b};
}

Vec2 vec2_div(Vec2 a, f32 b) {
    return (Vec2){.x = a.x / b, .y = a.y / b};
}

f32 vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

f32 vec2_mag(Vec2 v) {
    return sqrtf(vec2_dot(v, v));
}

Vec2 vec2_normalize(Vec2 v) {
    return vec2_div(v, vec2_mag(v));
}

Vec3 vec3(f32 x, f32 y, f32 z) {
    return (Vec3){.x = x, .y = y, .z = z};
}

Vec3 vec3_neg(Vec3 v) {
    return (Vec3){.x = -v.x, .y = -v.y, .z = -v.z};
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
}

Vec3 vec3_mul(Vec3 a, f32 b) {
    return (Vec3){.x = a.x * b, .y = a.y * b, .z = a.z * b};
}

Vec3 vec3_div(Vec3 a, f32 b) {
    return (Vec3){.x = a.x / b, .y = a.y / b, .z = a.z / b};
}

f32 vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 vec3_mag(Vec3 v) {
    return sqrtf(vec3_dot(v, v));
}

Vec3 vec3_normalize(Vec3 v) {
    return vec3_div(v, vec3_mag(v));
}

Vec4 vec4(f32 x, f32 y, f32 z, f32 w) {
    return (Vec4){.x = x, .y = y, .z = z, .w = w};
}

Vec4 vec4_neg(Vec4 v) {
    return (Vec4){.x = -v.x, .y = -v.y, .z = -v.z, .w = -v.w};
}

Vec4 vec4_add(Vec4 a, Vec4 b) {
    return (Vec4){.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w};
}

Vec4 vec4_sub(Vec4 a, Vec4 b) {
    return (Vec4){.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w};
}

Vec4 vec4_mul(Vec4 a, f32 b) {
    return (Vec4){.x = a.x * b, .y = a.y * b, .z = a.z * b, .w = a.w * b};
}

Vec4 vec4_div(Vec4 a, f32 b) {
    return (Vec4){.x = a.x / b, .y = a.y / b, .z = a.z / b, .w = a.w / b};
}

f32 vec4_dot(Vec4 a, Vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

f32 vec4_mag(Vec4 v) {
    return sqrtf(vec4_dot(v, v));
}

Vec4 vec4_normalize(Vec4 v) {
    return vec4_div(v, vec4_mag(v));
}

bool sampler_new(Sampler *sampler, usize width, usize height) {
    *sampler = (Sampler){.data = (u8 *)malloc(height * width * 3), .width = width, .height = height};
    if (!sampler->data) {
        fprintf(stderr, "Error allocating for sampler\n");
        return false;
    }
    return true;
}

Vec4 sampler_sample_nearest(const Sampler *sampler, f32 x, f32 y) {
    isize sx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    isize sy = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    f32 r = sampler->data[(sampler->width * sy + sx) * 3 + 0] / 255.0;
    f32 g = sampler->data[(sampler->width * sy + sx) * 3 + 1] / 255.0;
    f32 b = sampler->data[(sampler->width * sy + sx) * 3 + 2] / 255.0;
    return vec4(r, g, b, 1.0);
}

Vec4 sampler_sample_bilinear(const Sampler *sampler, f32 x, f32 y) {
    isize sx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    isize sy = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    f32 r = sampler->data[(sampler->width * sy + sx) * 3 + 0] / 255.0;
    f32 g = sampler->data[(sampler->width * sy + sx) * 3 + 1] / 255.0;
    f32 b = sampler->data[(sampler->width * sy + sx) * 3 + 2] / 255.0;
    return vec4(r, g, b, 1.0);
}

Vec4 sampler_sample_trilinear(const Sampler *sampler, f32 x, f32 y) {
    isize sx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    isize sy = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    f32 r = sampler->data[(sampler->width * sy + sx) * 3 + 0] / 255.0;
    f32 g = sampler->data[(sampler->width * sy + sx) * 3 + 1] / 255.0;
    f32 b = sampler->data[(sampler->width * sy + sx) * 3 + 2] / 255.0;
    return vec4(r, g, b, 1.0);
}

Vec4 sample(const Sampler *sampler, SamplerMethod method, f32 x, f32 y) {
    switch (method) {
    case Nearest:
        return sampler_sample_nearest(sampler, x, y);
        break;
    case Bilinear:
        return sampler_sample_bilinear(sampler, x, y);
        break;
    case Trilinear:
        return sampler_sample_trilinear(sampler, x, y);
        break;
    }
}

u8 *sampler_get(Sampler *sampler, usize x, usize y) {
    return &sampler->data[(sampler->width * y + x) * 3];
}

bool sampler_clone(const Sampler *from, Sampler *target) {
    return sampler_new(target, from->width, from->height);
}

bool sampler_load(char *path, Sampler *sampler) {
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

bool sampler_write(char *path, Sampler *sampler) {
    i32 success = stbi_write_png(path, sampler->width, sampler->height, 3, sampler->data, sampler->width * 3);
    if (!success) {
        fprintf(stderr, "Error writing sampler: %s\n", path);
        return false;
    }
    return true;
}

void sampler_free(Sampler sampler) {
    stbi_image_free(sampler.data);
}
