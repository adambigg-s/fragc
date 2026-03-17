#ifndef MATH_OVERLOADS_H
#define MATH_OVERLOADS_H

#include "shader.h"

static inline Vec2 operator-(const Vec2 v) {
    return vec2_neg(v);
}

static inline Vec2 operator+(const Vec2 a, const Vec2 b) {
    return vec2_add(a, b);
}

static inline Vec2 operator-(const Vec2 a, const Vec2 b) {
    return vec2_sub(a, b);
}

static inline Vec2 operator*(const Vec2 v, const f32 s) {
    return vec2_mul(v, s);
}

static inline f32 operator*(const Vec2 a, const Vec2 b) {
    return vec2_dot(a, b);
}

static inline Vec2 operator/(const Vec2 v, const f32 s) {
    return vec2_div(v, s);
}

static inline Vec3 operator-(const Vec3 v) {
    return vec3_neg(v);
}

static inline Vec3 operator+(const Vec3 a, const Vec3 b) {
    return vec3_add(a, b);
}

static inline Vec3 operator-(const Vec3 a, const Vec3 b) {
    return vec3_sub(a, b);
}

static inline Vec3 operator*(const Vec3 v, const f32 s) {
    return vec3_mul(v, s);
}

static inline f32 operator*(const Vec3 a, const Vec3 b) {
    return vec3_dot(a, b);
}

static inline Vec3 operator/(const Vec3 v, const f32 s) {
    return vec3_div(v, s);
}

static inline Vec4 operator-(const Vec4 v) {
    return vec4_neg(v);
}

static inline Vec4 operator+(const Vec4 a, const Vec4 b) {
    return vec4_add(a, b);
}

static inline Vec4 operator-(const Vec4 a, const Vec4 b) {
    return vec4_sub(a, b);
}

static inline Vec4 operator*(const Vec4 v, const f32 s) {
    return vec4_mul(v, s);
}

static inline f32 operator*(const Vec4 a, const Vec4 b) {
    return vec4_dot(a, b);
}

static inline Vec4 operator/(const Vec4 v, const f32 s) {
    return vec4_div(v, s);
}

static inline Mat2 operator*(const Mat2 a, const f32 b) {
    return mat2_muls(a, b);
}

static inline Vec2 operator*(const Mat2 m, const Vec2 v) {
    return mat2_mulv(m, v);
}

static inline Mat2 operator*(const Mat2 a, const Mat2 b) {
    return mat2_mulm(a, b);
}

static inline Mat3 operator*(const Mat3 a, const f32 b) {
    return mat3_muls(a, b);
}

static inline Vec3 operator*(const Mat3 m, const Vec3 v) {
    return mat3_mulv(m, v);
}

static inline Mat3 operator*(const Mat3 a, const Mat3 b) {
    return mat3_mulm(a, b);
}

static inline Mat4 operator*(const Mat4 a, const f32 b) {
    return mat4_muls(a, b);
}

static inline Vec4 operator*(const Mat4 m, const Vec4 v) {
    return mat4_mulv(m, v);
}

static inline Mat4 operator*(const Mat4 a, const Mat4 b) {
    return mat4_mulm(a, b);
}

#endif
