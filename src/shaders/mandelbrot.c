#include "../shader.h"

Vec2 mandel(Vec2 z, Vec2 c) {
    return vec2_add(mat2_mulv((Mat2){z.x, -z.y, z.y, z.x}, z), c);
}

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    f32 aspect = (f32)uniform->vp_width / (f32)uniform->vp_height;
    f32 zoom = 0.0015;
    Vec2 center = vec2(-0.74580, 0.100);
    Vec2 p = vec2((uv.x * 2.0 - 1.0) * aspect, uv.y * 2.0 - 1.0);
    Vec2 c = vec2_add(vec2_mul(p, zoom), center);
    Vec2 z = vec2(0.0, 0.0);
    bool escaped = false;
    for (usize i = 0; i < 500; i++) {
        z = mandel(z, c);
        if (vec2_dot(z, z) > 4.0) {
            escaped = true;
            break;
        }
    }

    return escaped ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
