#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vec3.h"
#include "lighting.h"
#include "animation.h"

#define WIDTH 512
#define HEIGHT 512
#define NUM_FRAMES 200

// Cube edges (centered)
#define CUBE_EDGES 12
int cube_edges[CUBE_EDGES][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},
    {4,5}, {5,6}, {6,7}, {7,4},
    {0,4}, {1,5}, {2,6}, {3,7}
};

vec3 cube_vertices[8] = {
    {-0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f}, {-0.5f,0.5f,-0.5f},
    {-0.5f,-0.5f,0.5f},  {0.5f,-0.5f,0.5f},  {0.5f,0.5f,0.5f},  {-0.5f,0.5f,0.5f}
};

// Pyramid edges (centered)
#define PYRAMID_EDGES 8
int pyramid_edges[PYRAMID_EDGES][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},
    {0,4}, {1,4}, {2,4}, {3,4}
};

vec3 pyramid_vertices[5] = {
    {-0.5f,0,-0.5f}, {0.5f,0,-0.5f}, {0.5f,0,0.5f}, {-0.5f,0,0.5f},
    {0,1,0}
};

// orthographic projection
void project(vec3 v, int *x, int *y) {
    *x = (int)(v.x * 40 + WIDTH / 2);
    *y = (int)(-v.y * 40 + HEIGHT / 2);
}

// bresenham line
void draw_line(int x0, int y0, int x1, int y1, unsigned char buffer[HEIGHT][WIDTH], unsigned char color) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = x0<x1 ? 1 : -1;
    int sy = y0<y1 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x0>=0 && x0<WIDTH && y0>=0 && y0<HEIGHT)
            buffer[y0][x0] = color;
        if (x0==x1 && y0==y1) break;
        int e2 = 2*err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

// rotations
vec3 rotate_x(vec3 v, float a) {
    float c=cosf(a),s=sinf(a);
    return (vec3){v.x,v.y*c - v.z*s,v.y*s+v.z*c};
}
vec3 rotate_y(vec3 v, float a) {
    float c=cosf(a),s=sinf(a);
    return (vec3){v.x*c+v.z*s,v.y,-v.x*s+v.z*c};
}
vec3 rotate_z(vec3 v, float a) {
    float c=cosf(a),s=sinf(a);
    return (vec3){v.x*c - v.y*s,v.x*s + v.y*c,v.z};
}

int main() {
    add_light((vec3){8,8,5});
    add_light((vec3){-8,8,5});

    const float radius = 2.0f;     // adjust for tighter or wider circle
    const float scale  = 2.0f;     // adjust to make shapes bigger

    for (int frame=0; frame<NUM_FRAMES; frame++) {
        unsigned char buffer[HEIGHT][WIDTH];
        memset(buffer,0,sizeof(buffer));

        float t = (float)frame/(NUM_FRAMES-1);
        float theta = t * 2.0f * M_PI;

        // center circular path
        vec3 motion = {
            radius * cosf(theta),
            radius * sinf(theta),
            0
        };

        // synced rotation
        float angle = theta;

        // cube
        vec3 cube[8];
        for (int i=0; i<8; i++) {
            vec3 v = cube_vertices[i];
            v.x *= scale;
            v.y *= scale;
            v.z *= scale;

            v = rotate_x(v,angle);
            v = rotate_y(v,angle);
            v = rotate_z(v,angle);

            cube[i] = (vec3){
                v.x + motion.x,
                v.y + motion.y,
                v.z + motion.z
            };
        }

        // pyramid (offset by +3 X from cube)
        vec3 pyramid[5];
        for (int i=0; i<5; i++) {
            vec3 v = pyramid_vertices[i];
            v.x *= scale;
            v.y *= scale;
            v.z *= scale;

            v = rotate_x(v,angle);
            v = rotate_y(v,angle);
            v = rotate_z(v,angle);

            pyramid[i] = (vec3){
                v.x + motion.x + 3.0f,
                v.y + motion.y,
                v.z + motion.z
            };
        }

        // draw cube
        for (int e=0; e<CUBE_EDGES; e++) {
            int a = cube_edges[e][0];
            int b = cube_edges[e][1];
            float intensity = compute_intensity(cube[a],cube[b]);
            int x0,y0,x1,y1;
            project(cube[a],&x0,&y0);
            project(cube[b],&x1,&y1);
            draw_line(x0,y0,x1,y1,buffer,(unsigned char)(intensity*255));
        }

        // draw pyramid
        for (int e=0; e<PYRAMID_EDGES; e++) {
            int a = pyramid_edges[e][0];
            int b = pyramid_edges[e][1];
            float intensity = compute_intensity(pyramid[a],pyramid[b]);
            int x0,y0,x1,y1;
            project(pyramid[a],&x0,&y0);
            project(pyramid[b],&x1,&y1);
            draw_line(x0,y0,x1,y1,buffer,(unsigned char)(intensity*255));
        }

        // save PGM
        char fname[64];
        sprintf(fname,"frame%03d.pgm",frame);
        FILE *f = fopen(fname,"w");
        fprintf(f,"P2\n%d %d\n255\n",WIDTH,HEIGHT);
        for (int y=0;y<HEIGHT;y++){
            for(int x=0;x<WIDTH;x++)
                fprintf(f,"%d ",buffer[y][x]);
            fprintf(f,"\n");
        }
        fclose(f);
        printf("Wrote %s\n",fname);
    }
    return 0;
}
