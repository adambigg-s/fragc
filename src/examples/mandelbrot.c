#include "../shader.h"
#include <math.h>

#define PI 3.141592

static const usize SUPER_SAMPLE = 16;
static const usize MAX_ITERS = 700;

Vec3 palette(f32 time) {
    // clang-format off
    return vec3(
        0.6 + 0.4 * cosf(2.0 * PI * (time + PI * 0.1)),
        0.4 + 0.6 * cosf(2.0 * PI * (time + PI * 0.4)),
        0.4 + 0.6 * cosf(2.0 * PI * (time + PI * 0.7))
    );
    // clang-format on
}

Vec2 m_fn(Vec2 z, Vec2 c) {
    return vec2_add(mat2_mulv((Mat2){z.x, -z.y, z.y, z.x}, z), c);
}

/* extern */ __declspec(dllexport) Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    f32 aspect = (f32)uniform->vp_width / (f32)uniform->vp_height;
    f32 zoom = 0.0015;
    Vec2 center = vec2(-0.7458, 0.1);
    Vec3 accumulator = vec3_zero();

    for (isize sy = 0; sy < SUPER_SAMPLE; sy++) {
        for (isize sx = 0; sx < SUPER_SAMPLE; sx++) {
            // clang-format off
            Vec2 offset = vec2(
                ((f32)sx + 0.5) / SUPER_SAMPLE,
                ((f32)sy + 0.5) / SUPER_SAMPLE
            );
            Vec2 super_uv = vec2(
                uv.x + (offset.x - 0.5) / uniform->vp_width,
                uv.y + (offset.y - 0.5) / uniform->vp_height
            );
            // clang-format on

            Vec2 p = vec2((super_uv.x * 2.0 - 1.0) * aspect, super_uv.y * 2.0 - 1.0);
            Vec2 c = vec2_add(vec2_mul(p, zoom), center);
            Vec2 z = vec2_zero();

            usize iters;
            for (iters = 0; iters < MAX_ITERS; iters++) {
                z = m_fn(z, c);
                if (vec2_dot(z, z) > 4.0) {
                    break;
                }
            }

            f32 color = (f32)iters / MAX_ITERS;
            accumulator = vec3_add(accumulator, palette(color));
        }
    }

    accumulator = vec3_mul(accumulator, 1.0 / (SUPER_SAMPLE * SUPER_SAMPLE));

    return vec4(accumulator.x, accumulator.y, accumulator.z, 1.0);
}
