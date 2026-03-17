#include "../shader.h"

// clang-format off
static const Mat4 DITHER = {.ele = {
    0  / 16.0, 8  / 16.0, 2  / 16.0, 10/ 16.0,
    12 / 16.0, 4  / 16.0, 14 / 16.0, 6 / 16.0,
    3  / 16.0, 11 / 16.0, 1  / 16.0, 9 / 16.0,
    15 / 16.0, 7  / 16.0, 13 / 16.0, 5 / 16.0,
}};
// clang-format on

f32 dither_thresh(usize x, usize y) {
    return mat4_get(&DITHER, y, x);
}

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    Vec4 col = sample(sampler, Nearest, uv.x, uv.y);

    usize x = (usize)(uv.x * sampler->width / 4) % 4;
    usize y = (usize)(uv.y * sampler->height / 4) % 4;

    f32 thresh = dither_thresh(x, y);

    Vec3 rgb = {};
    rgb.x = col.x * (1.0 + 0.55 * thresh);
    rgb.y = col.y * (1.0 + 0.55 * thresh);
    rgb.z = col.z * (1.0 + 0.55 * thresh);

    return vec4(rgb.x, rgb.y, rgb.z, col.w);
}
