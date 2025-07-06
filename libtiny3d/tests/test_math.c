#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math3d.h"

#define WIDTH 256
#define HEIGHT 256
#define FRAMES 60

unsigned char canvas[HEIGHT][WIDTH];

void clear() {
    memset(canvas, 255, sizeof(canvas));  // white background
}

void plot(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        canvas[y][x] = 0;  // black line
}

void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        plot(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void save_pgm(const char* filename) {
    FILE* f = fopen(filename, "wb");
    fprintf(f, "P2\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++)
            fprintf(f, "%d ", canvas[y][x]);
        fprintf(f, "\n");
    }
    fclose(f);
}

int main() {
    vec3_t cube[] = {
        {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
        {-1,-1,1},  {1,-1,1},  {1,1,1},  {-1,1,1}
    };

    int edges[][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for (int frame = 0; frame < FRAMES; frame++) {
        clear();

        // animate the angles
        float rx = frame * 0.05f;  // slowly rotate
        float ry = frame * 0.04f;
        float rz = frame * 0.03f;

        mat4_t rot_x = mat4_rotate_x(rx);
        mat4_t rot_y = mat4_rotate_y(ry);
        mat4_t rot_z = mat4_rotate_z(rz);

        mat4_t rot = mat4_mul(mat4_mul(rot_z, rot_y), rot_x);
        mat4_t trans = mat4_translate(0, 0, 5);

        int px[8], py[8];
        for (int i = 0; i < 8; i++) {
            vec3_t p = mat4_mul_vec3(rot, cube[i]);
            p = mat4_mul_vec3(trans, p);

            // perspective
            int x = (int)((p.x / p.z) * 100 + WIDTH / 2);
            int y = (int)((p.y / p.z) * 100 + HEIGHT / 2);
            px[i] = x;
            py[i] = y;
        }

        for (int i = 0; i < 12; i++) {
            draw_line(px[edges[i][0]], py[edges[i][0]],
                      px[edges[i][1]], py[edges[i][1]]);
        }

        char filename[64];
        sprintf(filename, "cube%03d.pgm", frame);
        save_pgm(filename);
        printf("Saved %s\n", filename);
    }

    printf("Done!\n");
    return 0;
}
