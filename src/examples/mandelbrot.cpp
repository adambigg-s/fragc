#include "../math_overloads.hpp"
#include "../shader.h"

static inline Vec2 m_fn(Vec2 z, Vec2 c) {
    return (Mat2){z.x, -z.y, z.y, z.x} * z + c;
}

extern "C" __declspec(dllexport) Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    f32 aspect = (f32)uniform->vp_width / (f32)uniform->vp_height;
    f32 zoom = 0.0015;
    Vec2 center = vec2(-0.74580, 0.100);
    Vec2 p = vec2((uv.x * 2.0 - 1.0) * aspect, uv.y * 2.0 - 1.0);
    Vec2 c = p * zoom + center;
    Vec2 z = vec2(0.0, 0.0);
    bool escaped = false;
    for (usize i = 0; i < 500; i++) {
        z = m_fn(z, c);
        if (z * z > 4.0) {
            escaped = true;
            break;
        }
    }

    return escaped ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
