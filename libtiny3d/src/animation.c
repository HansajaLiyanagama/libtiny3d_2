#include "animation.h"

vec3 cubic_bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {
    float u = 1.0f - t;
    float uu = u * u;
    float uuu = uu * u;
    float tt = t * t;
    float ttt = tt * t;

    vec3 res;
    res.x = uuu*p0.x + 3*uu*t*p1.x + 3*u*tt*p2.x + ttt*p3.x;
    res.y = uuu*p0.y + 3*uu*t*p1.y + 3*u*tt*p2.y + ttt*p3.y;
    res.z = uuu*p0.z + 3*uu*t*p1.z + 3*u*tt*p2.z + ttt*p3.z;

    return res;
}
