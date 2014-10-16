#ifndef PHYSICS_HEADER
#define PHYSICS_HEADER

#include <chipmunk/chipmunk.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

//Should stay between .01 and .25; Square distance in meters
#define THRESHOLD .02 
#define INITIAL 0.000001
#define SCALE 100

#define CRAYON_MASS .5
#define CRAYON_FRICTION 0.3
#define CRAYON_RADIUS 0.015

/*
 * struct that hold the vertices of the current drawing with two GArrays
 * also holds a long to represent the color of the drawing
 */
typedef struct body_data Body_data;
struct body_data {
  GArray *x_values, *y_values;
  long color_rgb;
};

typedef struct shape_data {
  int type;
} Shape_data;

/**
 *  On mouse press
 * Starts the drawing by creating a cpBody
 * Calls the constructor for a Point_array and stores it in the UserData parameter for the cpBody
 * Initializes the Point_array with the coordinates and the color
 *
 * @param int x value in world coordinates (not mouse coordinates)
 * @param int y value
 * @param long color_rgb that stores an ARGB value in 32 bits
 * 
 * @return cpBody* with a Point_array in the UserData
 */
cpBody* drawing_new(cpFloat x, cpFloat y, long color_rgb);

/** 
 *  On mouse drag
 * Adds a new segment to the drawing if the mouse has been dragged past THRESHOLD
 * Creates a new cpShape to add to the body
 * Updates mass, moment, and center of mass of the body
 *
 * @param cpSpace *space that is holding the body
 * @param cpBody *drawing to be updated
 * @param int x value in world coordinates
 * @param int y value
 * 
 * @return cpBody* that has an added segment and updated properties
 */
cpBody* drawing_update(cpSpace *space, cpBody *drawing, cpFloat x, cpFloat y);

/**
 *  On mouse release
 * Unsleeps the cpBody so that it will begin to interact with the space
 * Also frees the GArrays in the UserData of the body but keeps color_rgb
 * because the arrays won't be used anymore but the color still needs to be drawm
 *
 * @param cpBody *drawing to be activated when it is done being drawn
 */
void drawing_activate(cpSpace *space, cpBody *drawing);

/**
 * Frees the body and all of its shapes
 * Also frees the Point_array in the user data
 *
 * @param cpBody *body to be freed
 */
void free_body_full(cpBody *body);

#endif
