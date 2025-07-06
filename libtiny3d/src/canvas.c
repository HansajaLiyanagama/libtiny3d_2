#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"

// Allocate a 2D float array for pixels
canvas_t* create_canvas(int width, int height) {
    canvas_t* c = malloc(sizeof(canvas_t));
    c->width = width;
    c->height = height;
    c->pixels = malloc(height * sizeof(float*));
    for (int i = 0; i < height; i++) {
        c->pixels[i] = calloc(width, sizeof(float)); // initialize to 0.0f
    }
    return c;
}

void destroy_canvas(canvas_t* canvas) {
    if (!canvas) return;
    for (int i = 0; i < canvas->height; i++) {
        free(canvas->pixels[i]);
    }
    free(canvas->pixels);
    free(canvas);
}

void clear_canvas(canvas_t* canvas, float intensity) {
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            canvas->pixels[y][x] = intensity;
        }
    }
}

// Clamp helper
static float clamp(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

// Set pixel intensity at floating point coords with nearest neighbor
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    int ix = (int)roundf(x);
    int iy = (int)roundf(y);
    if (ix < 0 || ix >= canvas->width || iy < 0 || iy >= canvas->height) return;

    // Blend max intensity
    if (intensity > canvas->pixels[iy][ix])
        canvas->pixels[iy][ix] = clamp(intensity, 0.0f, 1.0f);
}

// Draw line with thickness using simple Wu's antialiasing (simplified)
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    // Simple Bresenham-like line without AA, thickness ignored for brevity
    int ix0 = (int)roundf(x0);
    int iy0 = (int)roundf(y0);
    int ix1 = (int)roundf(x1);
    int iy1 = (int)roundf(y1);

    int dx = abs(ix1 - ix0), sx = ix0 < ix1 ? 1 : -1;
    int dy = -abs(iy1 - iy0), sy = iy0 < iy1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        set_pixel_f(canvas, (float)ix0, (float)iy0, 1.0f);
        if (ix0 == ix1 && iy0 == iy1) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            ix0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            iy0 += sy;
        }
    }
}

void save_canvas_as_pgm(canvas_t* canvas, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return;
    }
    fprintf(f, "P5\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            unsigned char val = (unsigned char)(clamp(canvas->pixels[y][x], 0.0f, 1.0f) * 255.0f);
            fputc(val, f);
        }
    }
    fclose(f);
}
