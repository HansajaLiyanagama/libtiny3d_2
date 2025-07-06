#ifndef CANVAS_H // Preprocessor directive: If CANVAS_H is not defined
#define CANVAS_H // Defined CANVAS_H to prevent multiple inclusions of the header file

// Define the structure for the canvas (As an object)
// The structure holds the dimensions and a 2D array of pixel data

typedef struct {
    int width;
    int height;
    float** pixels;  // 2D array (float**) to store intensity values of pixels
} canvas_t;

// Functions that are implemented in canvas.c
// Also these functions can be used by other parts of the program that include this header


// Allocate memory for a new canvas object and its pixel data
// Return a pointer to the canvas_t ,or NULL if allocation failed 
canvas_t* create_canvas(int width, int height); 

// Deallocates all memory associated with a canvas object
// Parameter: A pointer to the canvas object
void destroy_canvas(canvas_t* canvas);

// Fills the entire canvas with a specified intensity value
void clear_canvas(canvas_t* canvas, float intensity);

// Sets the pixel intensity at floating point coordinates using nearest neighbour rounding
// The pixel's intensity is updated only if the new intensity is greater than the current intensity
void set_pixel_f(canvas_t* canvas, float x, float y, float intensity);

// Drawing a line on the canvas (Uses simplified algorithm similar to Bresenham algorithm)
void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness);

// Saves the current canvas content as a PGM (Portable Graymap) image file
void save_canvas_as_pgm(canvas_t* canvas, const char* filename);

#endif // CANVAS_H
