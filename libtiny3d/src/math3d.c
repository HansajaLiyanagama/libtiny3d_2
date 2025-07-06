// math3d.c
#include "math3d.h"
#include <math.h>

#define DEG2RAD(x) ((x) * M_PI / 180.0f)

vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;
    v.r = r;
    v.theta = theta;
    v.phi = phi;
    v.x = r * sinf(theta) * cosf(phi);
    v.y = r * sinf(theta) * sinf(phi);
    v.z = r * cosf(theta);
    return v;
}

void vec3_normalize_fast(vec3_t* v) {
    float inv_len = 1.0f / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x *= inv_len;
    v->y *= inv_len;
    v->z *= inv_len;
}

vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    vec3_normalize_fast(&a);
    vec3_normalize_fast(&b);
    float dot = a.x * b.x + a.y * b.y + a.z * b.z;
    float theta = acosf(dot) * t;
    vec3_t rel = {b.x - a.x * dot, b.y - a.y * dot, b.z - a.z * dot};
    vec3_normalize_fast(&rel);

    vec3_t result = {
        a.x * cosf(theta) + rel.x * sinf(theta),
        a.y * cosf(theta) + rel.y * sinf(theta),
        a.z * cosf(theta) + rel.z * sinf(theta)
    };
    return result;
}

// Matrix section
mat4_t mat4_identity() {
    mat4_t m = {0};
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[12] = tx; m.m[13] = ty; m.m[14] = tz;
    return m;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = mat4_identity();
    m.m[0] = sx; m.m[5] = sy; m.m[10] = sz;
    return m;
}

mat4_t mat4_rotate_x(float angle) {
    mat4_t m = mat4_identity();
    float s = sinf(angle), c = cosf(angle);
    m.m[5] = c; m.m[6] = s;
    m.m[9] = -s; m.m[10] = c;
    return m;
}

mat4_t mat4_rotate_y(float angle) {
    mat4_t m = mat4_identity();
    float s = sinf(angle), c = cosf(angle);
    m.m[0] = c; m.m[2] = -s;
    m.m[8] = s; m.m[10] = c;
    return m;
}

mat4_t mat4_rotate_z(float angle) {
    mat4_t m = mat4_identity();
    float s = sinf(angle), c = cosf(angle);
    m.m[0] = c; m.m[1] = s;
    m.m[4] = -s; m.m[5] = c;
    return m;
}

vec3_t mat4_mul_vec3(mat4_t m, vec3_t v) {
    vec3_t r;
    r.x = m.m[0]*v.x + m.m[4]*v.y + m.m[8]*v.z + m.m[12];
    r.y = m.m[1]*v.x + m.m[5]*v.y + m.m[9]*v.z + m.m[13];
    r.z = m.m[2]*v.x + m.m[6]*v.y + m.m[10]*v.z + m.m[14];
    return r;
}
mat4_t mat4_mul(mat4_t a, mat4_t b) {
    mat4_t result = {0};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                result.m[col + row * 4] += a.m[k + row * 4] * b.m[col + k * 4];
            }
        }
    }
    return result;
}

