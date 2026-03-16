#include "../shader.h"

Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    return sample(sampler, Nearest, uv.x, uv.y);
}
