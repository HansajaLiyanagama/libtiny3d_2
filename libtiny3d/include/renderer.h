#ifndef RENDERER_H
#define RENDERER_H

#include "canvas.h"

typedef struct { float x, y, z; } Vec3;
typedef struct { int a, b; } Edge;

void project_vertex(Vec3 p, int *x, int *y);
int clip_to_circular_viewport(int x, int y);
void render_wireframe(canvas_t* canvas, Vec3 *transformed, Edge *edges, int vertex_count, int edge_count);

#endif
