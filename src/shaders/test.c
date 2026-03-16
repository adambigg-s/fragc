#include "../shader.h"

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    return sampler_sample(sampler, uv.x, uv.y);
}
