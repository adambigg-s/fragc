#include "shader.h"

bool sampler_new(Sampler *sampler, usize width, usize height) {
    *sampler = (Sampler){.data = (u8 *)malloc(height * width * 3), .width = width, .height = height};
    if (!sampler->data) {
        fprintf(stderr, "Error allocating for sampler\n");
        return false;
    }
    return true;
}

u8 *sampler_get(Sampler *sampler, usize x, usize y) {
    return &sampler->data[(sampler->width * y + x) * 3];
}

Vec3 sampler_get_slice(const Sampler *sampler, usize x, usize y) {
    u8 *ptr = &sampler->data[(sampler->width * y + x) * 3];
    return vec3_div(vec3(ptr[0], ptr[1], ptr[2]), 255.0);
}

Vec4 sampler_sample_nearest(const Sampler *sampler, f32 x, f32 y) {
    isize sx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    isize sy = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    Vec3 color = sampler_get_slice(sampler, sx, sy);
    return vec4(color.x, color.y, color.z, 1.0);
}

Vec4 sampler_sample_bilinear(const Sampler *sampler, f32 x, f32 y) {
    f32 tx = clampx(x * (sampler->width - 1), 0.0, sampler->width - 1);
    f32 ty = clampx(y * (sampler->height - 1), 0.0, sampler->height - 1);
    f32 fx = fractx(tx);
    f32 fy = fractx(ty);
    isize sx = tx;
    isize sy = ty;

    Vec3 v00 = sampler_get_slice(sampler, sx, sy);
    Vec3 v01 = sampler_get_slice(sampler, sx + 1, sy);
    Vec3 v10 = sampler_get_slice(sampler, sx, sy + 1);
    Vec3 v11 = sampler_get_slice(sampler, sx + 1, sy + 1);
    f32 w00 = fx * fy;
    f32 w01 = (1.0 - fx) * fy;
    f32 w10 = fx * (1.0 - fy);
    f32 w11 = (1.0 - fx) * (1.0 - fy);

    Vec3 color = vec3_add(
        vec3_add(vec3_add(vec3_mul(v00, w00), vec3_mul(v01, w01)), vec3_mul(v10, w10)), vec3_mul(v11, w11));

    return vec4(color.x, color.y, color.z, 1.0);
}

Vec4 sampler_sample_trilinear(const Sampler *sampler, f32 x, f32 y) {
    return vec4(0.0, 0.0, 0.0, 0.0);
}

Vec4 sample(const Sampler *sampler, SamplerMethod method, f32 x, f32 y) {
    switch (method) {
    case Nearest:
        return sampler_sample_nearest(sampler, x, y);
        break;
    case Bilinear:
        return sampler_sample_bilinear(sampler, x, y);
        break;
    case Trilinear:
        return sampler_sample_trilinear(sampler, x, y);
        break;
    }
}

bool sampler_clone(const Sampler *from, Sampler *target) {
    return sampler_new(target, from->width, from->height);
}

bool sampler_load(char *path, Sampler *sampler) {
    i32 width, height, levels;
    u8 *data = stbi_load(path, &width, &height, &levels, 3);
    if (!data) {
        fprintf(stderr, "Error loading sampler: %s\n", path);
        return false;
    }
    sampler->data = data;
    sampler->height = height;
    sampler->width = width;
    return true;
}

bool sampler_write(char *path, Sampler *sampler) {
    i32 success = stbi_write_png(path, sampler->width, sampler->height, 3, sampler->data, sampler->width * 3);
    if (!success) {
        fprintf(stderr, "Error writing sampler: %s\n", path);
        return false;
    }
    return true;
}

void sampler_free(Sampler sampler) {
    stbi_image_free(sampler.data);
}
