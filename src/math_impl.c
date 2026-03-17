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

f32 mat2_get(const Mat2 *mat, usize i, usize j) {
    return mat->ele[2 * i + j];
}

f32 *mat2_get_mut(Mat2 *mat, usize i, usize j) {
    return &mat->ele[2 * i + j];
}

Mat2 mat2_muls(Mat2 m, f32 s) {
    Mat2 res = {0};
    for (usize i = 0; i < 4; i++) {
        res.ele[i] = m.ele[i] * s;
    }
    return res;
}

Vec2 mat2_mulv(Mat2 m, Vec2 v) {
    // clang-format off
    return vec2(
        vec2_dot(vec2(mat2_get(&m, 0, 0), mat2_get(&m, 0, 1)), v),
        vec2_dot(vec2(mat2_get(&m, 1, 0), mat2_get(&m, 1, 1)), v)
    );
    // clang-format on
}

Mat2 mat2_mulm(Mat2 a, Mat2 b) {
    Mat2 res = {0};
    for (usize i = 0; i < 2; i++) {
        for (usize j = 0; j < 2; j++) {
            for (usize k = 0; k < 2; k++) {
                *mat2_get_mut(&res, i, j) += mat2_get(&a, i, k) * mat2_get(&b, k, j);
            }
        }
    }
    return res;
}

f32 mat3_get(const Mat3 *mat, usize i, usize j) {
    return mat->ele[3 * i + j];
}

f32 *mat3_get_mut(Mat3 *mat, usize i, usize j) {
    return &mat->ele[3 * i + j];
}

Mat3 mat3_muls(Mat3 m, f32 s) {
    Mat3 res = {0};
    for (usize i = 0; i < 4; i++) {
        res.ele[i] = m.ele[i] * s;
    }
    return res;
}

Vec3 mat3_mulv(Mat3 m, Vec3 v) {
    // clang-format off
    return vec3(
        vec3_dot(vec3(mat3_get(&m, 0, 0), mat3_get(&m, 0, 1), mat3_get(&m, 0, 2)), v),
        vec3_dot(vec3(mat3_get(&m, 1, 0), mat3_get(&m, 1, 1), mat3_get(&m, 1, 2)), v),
        vec3_dot(vec3(mat3_get(&m, 2, 0), mat3_get(&m, 2, 1), mat3_get(&m, 2, 2)), v)
    );
    // clang-format on
}

Mat3 mat3_mulm(Mat3 a, Mat3 b) {
    Mat3 res = {0};
    for (usize i = 0; i < 3; i++) {
        for (usize j = 0; j < 3; j++) {
            for (usize k = 0; k < 3; k++) {
                *mat3_get_mut(&res, i, j) += mat3_get(&a, i, k) * mat3_get(&b, k, j);
            }
        }
    }
    return res;
}

f32 mat4_get(const Mat4 *mat, usize i, usize j) {
    return mat->ele[4 * i + j];
}

f32 *mat4_get_mut(Mat4 *mat, usize i, usize j) {
    return &mat->ele[4 * i + j];
}

Mat4 mat4_muls(Mat4 m, f32 s) {
    Mat4 res = {0};
    for (usize i = 0; i < 4; i++) {
        res.ele[i] = m.ele[i] * s;
    }
    return res;
}

Vec4 mat4_mulv(Mat4 m, Vec4 v) {
    // clang-format off
    return vec4(
        vec4_dot(vec4(mat4_get(&m, 0, 0), mat4_get(&m, 0, 1), mat4_get(&m, 0, 2), mat4_get(&m, 0, 3)), v),
        vec4_dot(vec4(mat4_get(&m, 1, 0), mat4_get(&m, 1, 1), mat4_get(&m, 1, 2), mat4_get(&m, 1, 3)), v),
        vec4_dot(vec4(mat4_get(&m, 2, 0), mat4_get(&m, 2, 1), mat4_get(&m, 2, 2), mat4_get(&m, 2, 3)), v),
        vec4_dot(vec4(mat4_get(&m, 3, 0), mat4_get(&m, 3, 1), mat4_get(&m, 3, 2), mat4_get(&m, 3, 3)), v)
    );
    // clang-format on
}

Mat4 mat4_mulm(Mat4 a, Mat4 b) {
    Mat4 res = {0};
    for (usize i = 0; i < 4; i++) {
        for (usize j = 0; j < 4; j++) {
            for (usize k = 0; k < 4; k++) {
                *mat4_get_mut(&res, i, j) += mat4_get(&a, i, k) * mat4_get(&b, k, j);
            }
        }
    }
    return res;
}
