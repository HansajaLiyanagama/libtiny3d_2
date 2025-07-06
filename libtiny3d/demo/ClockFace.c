#include <math.h>      // For sin, cos, and PI
#include "canvas.h"    // Our canvas functions and struct

// If M_PI is not defined by your system, define it manually
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main() {
    int width = 512, height = 512;           // Canvas size
    canvas_t* canvas = create_canvas(width, height); // Create blank canvas

    clear_canvas(canvas, 0.0f);              // Fill canvas with black (intensity = 0)

    float cx = width / 2.0f;                 // Center x
    float cy = height / 2.0f;                // Center y
    float radius = 200.0f;                   // Radius for the lines

    // Draw lines at 15° intervals like a clock
    for (int angle_deg = 0; angle_deg < 360; angle_deg += 15) {
        float rad = angle_deg * M_PI / 180.0f;   // Convert degrees to radians
        float x = cx + radius * cos(rad);       // End x position
        float y = cy + radius * sin(rad);       // End y position

        draw_line_f(canvas, cx, cy, x, y, 2.0f); // Draw line from center to (x, y)
    }

    // Save result as a PGM image
    save_canvas_as_pgm(canvas, "output.pgm");

    // Clean up memory
    destroy_canvas(canvas);

    return 0;
}
