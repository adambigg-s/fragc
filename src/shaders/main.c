#include "../shader.h"

/* extern */ __declspec(dllexport) Vec4 frag(const Uniform *uniform, const Sampler *sampler, Vec2 uv) {
    return sample(sampler, Bilinear, uv.x, uv.y); // Just a color pass-through
}
