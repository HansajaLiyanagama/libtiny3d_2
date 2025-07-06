#include <stdio.h>
#include "renderer.h"

#define WIDTH 500
#define HEIGHT 500

void project_vertex(Vec3 p, int *x, int *y) {
    float scale = 100.0f;
    *x = WIDTH / 2 + (int)(p.x * scale);
    *y = HEIGHT / 2 - (int)(p.y * scale);
}

int clip_to_circular_viewport(int x, int y) {
    int dx = x - WIDTH / 2;
    int dy = y - HEIGHT / 2;
    return dx * dx + dy * dy <= (WIDTH / 2) * (WIDTH / 2);
}

void render_wireframe(canvas_t* canvas, Vec3 *transformed, Edge *edges, int vertex_count, int edge_count) {
    for (int i = 0; i < edge_count; i++) {
        int x0, y0, x1, y1;
        project_vertex(transformed[edges[i].a], &x0, &y0);
        project_vertex(transformed[edges[i].b], &x1, &y1);

        if (!clip_to_circular_viewport(x0, y0) || !clip_to_circular_viewport(x1, y1))
            continue;

        draw_line_f(canvas, (float)x0, (float)y0, (float)x1, (float)y1, 1.0f);
    }
}
