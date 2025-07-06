#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "canvas.h"
#include "renderer.h"

#define MAX_VERTICES 1024
#define MAX_EDGES 2048
#define FRAMES 60
#define PI 3.14159265f

// -----------------------------------
// types
// -----------------------------------

typedef struct {
    float w, x, y, z;
} Quaternion;

// -----------------------------------
// globals
// -----------------------------------

Vec3 vertices[MAX_VERTICES];
Edge edges[MAX_EDGES];
int vertex_count = 0;
int edge_count = 0;

// -----------------------------------
// quaternion helpers
// -----------------------------------

Quaternion quat_from_axis_angle(Vec3 axis, float angle) {
    float half = angle * 0.5f;
    float s = sinf(half);
    return (Quaternion){cosf(half), axis.x * s, axis.y * s, axis.z * s};
}

Vec3 quat_rotate_vec3(Quaternion q, Vec3 v) {
    // q * v * q^-1
    Quaternion p = {0, v.x, v.y, v.z};
    Quaternion q_conj = {q.w, -q.x, -q.y, -q.z};
    Quaternion qp;

    // qp = q * p
    qp.w = q.w*p.w - q.x*p.x - q.y*p.y - q.z*p.z;
    qp.x = q.w*p.x + q.x*p.w + q.y*p.z - q.z*p.y;
    qp.y = q.w*p.y - q.x*p.z + q.y*p.w + q.z*p.x;
    qp.z = q.w*p.z + q.x*p.y - q.y*p.x + q.z*p.w;

    // result = qp * q_conj
    Quaternion res;
    res.w = qp.w*q_conj.w - qp.x*q_conj.x - qp.y*q_conj.y - qp.z*q_conj.z;
    res.x = qp.w*q_conj.x + qp.x*q_conj.w + qp.y*q_conj.z - qp.z*q_conj.y;
    res.y = qp.w*q_conj.y - qp.x*q_conj.z + qp.y*q_conj.w + qp.z*q_conj.x;
    res.z = qp.w*q_conj.z + qp.x*q_conj.y - qp.y*q_conj.x + qp.z*q_conj.w;

    return (Vec3){res.x, res.y, res.z};
}

// -----------------------------------
// OBJ loader
// -----------------------------------

void load_obj(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("fopen");
        exit(1);
    }

    vertex_count = 0;
    edge_count = 0;
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == 'v' && line[1] == ' ') {
            if (vertex_count >= MAX_VERTICES) {
                fprintf(stderr, "Too many vertices\n");
                exit(1);
            }
            Vec3 v;
            if (sscanf(line + 2, "%f %f %f", &v.x, &v.y, &v.z) == 3) {
                vertices[vertex_count++] = v;
            }
        } else if (line[0] == 'f') {
            int indices[16], n = 0;
            char *token = strtok(line + 2, " ");
            while (token && n < 16) {
                int idx = 0;
                sscanf(token, "%d", &idx);
                if (idx == 0) {
                    char *slash = strchr(token, '/');
                    if (slash) {
                        char tmp[16];
                        int len = slash - token;
                        if (len >= (int)sizeof(tmp)) len = sizeof(tmp) - 1;
                        strncpy(tmp, token, len);
                        tmp[len] = '\0';
                        idx = atoi(tmp);
                    }
                }
                indices[n++] = idx;
                token = strtok(NULL, " ");
            }

            for (int i = 0; i < n; i++) {
                int a = indices[i] - 1;
                int b = indices[(i + 1) % n] - 1;
                if (a < 0 || a >= vertex_count || b < 0 || b >= vertex_count) {
                    fprintf(stderr, "Face index out of range: %d/%d (n=%d)\n", a, b, n);
                    exit(1);
                }
                if (edge_count >= MAX_EDGES) {
                    fprintf(stderr, "Too many edges\n");
                    exit(1);
                }
                edges[edge_count++] = (Edge){a, b};
            }
        }
    }

    fclose(f);
    printf("Loaded OBJ: %d vertices, %d edges\n", vertex_count, edge_count);
}

// -----------------------------------
// main render loop
// -----------------------------------

int main() {
    canvas_t* canvas = create_canvas(512, 512);

    load_obj("soccer.obj");

    for (int frame = 0; frame < FRAMES; frame++) {
        clear_canvas(canvas, 0.0f);

        float angle = 2 * PI * frame / FRAMES;
        Quaternion q = quat_from_axis_angle((Vec3){0, 1, 0}, angle); // rotate around y axis

        Vec3 rotated[MAX_VERTICES];
        for (int i = 0; i < vertex_count; i++) {
            rotated[i] = quat_rotate_vec3(q, vertices[i]);
        }

        render_wireframe(canvas, rotated, edges, vertex_count, edge_count);

        char filename[64];
        sprintf(filename, "frame_%03d.pgm", frame);
        save_canvas_as_pgm(canvas, filename);
    }

    destroy_canvas(canvas);
    printf("Rendered %d frames successfully.\n", FRAMES);
    return 0;
}
