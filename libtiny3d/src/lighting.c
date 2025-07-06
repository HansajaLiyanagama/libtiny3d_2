#include <math.h>
#include "lighting.h"

#define MAX_LIGHTS 8

static vec3 lights[MAX_LIGHTS];
static int num_lights = 0;

vec3 vec3_sub(vec3 a, vec3 b) {
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec3_add(vec3 a, vec3 b) {
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3_scale(vec3 v, float s) {
    return (vec3){v.x * s, v.y * s, v.z * s};
}

vec3 vec3_normalize(vec3 v) {
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (len > 0.0001f)
        return vec3_scale(v, 1.0f/len);
    return v;
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

void add_light(vec3 pos) {
    if (num_lights < MAX_LIGHTS) {
        lights[num_lights++] = pos;
    }
}

float compute_intensity(vec3 p1, vec3 p2) {
    vec3 edge_dir = vec3_normalize(vec3_sub(p2, p1));
    vec3 midpoint = vec3_scale(vec3_add(p1, p2), 0.5f);
    float total_intensity = 0.0f;

    for (int i = 0; i < num_lights; i++) {
        vec3 light_dir = vec3_normalize(vec3_sub(lights[i], midpoint));
        total_intensity += fmaxf(0.0f, vec3_dot(edge_dir, light_dir));
    }

    if (total_intensity > 1.0f) total_intensity = 1.0f;
    return total_intensity;
}
