#include "../shader.h"

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    Vec4 col = sample(sampler, Nearest, uv.x, uv.y);
    // return sample(sampler, Nearest, uv.x, uv.y);
    return (col + vec4(1.0, 1.0, 1.0, 1.0)) / 2.0;
}
