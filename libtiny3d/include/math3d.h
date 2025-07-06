// math3d.h
#ifndef MATH3D_H
#define MATH3D_H

typedef struct {
    float x, y, z;
    float r, theta, phi; // Spherical
} vec3_t;

typedef struct {
    float m[16]; // Column-major
} mat4_t;

vec3_t vec3_from_spherical(float r, float theta, float phi);
void vec3_normalize_fast(vec3_t* v);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);

mat4_t mat4_identity();
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate_x(float angle);
mat4_t mat4_rotate_y(float angle);
mat4_t mat4_rotate_z(float angle);
mat4_t mat4_mul(mat4_t a, mat4_t b);


vec3_t mat4_mul_vec3(mat4_t m, vec3_t v);

#endif
