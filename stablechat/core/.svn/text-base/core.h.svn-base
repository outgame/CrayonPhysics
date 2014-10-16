#ifndef PHYSICS_HEADER
#define PHYSICS_HEADER

/**
 * \file core.h
 * \brief Central physics interface with Chipmunk library
 *
 * Core.h is visible only to files requiring communication with chipmunk
 * and its structures like cpBody, cpShape and cpSpace. This includes both
 * 1-player graphics and the server-side for multiplayer.
 *
 */

#include <chipmunk.h>
#include <gtk/gtk.h>

/** Should stay between .01 and .25, in meters (Square distance) */
#define THRESHOLD .01
/** Because chipmunk does not allow 0 for many fields */
#define INITIAL 0.000001
/** Pixels per meter */
#define SCALE 100

/** CRAYON_ constants are used as standards for the entire gameworld */
#define CRAYON_MASS .5
#define CRAYON_FRICTION 0.33
#define CRAYON_RADIUS 0.05
#define CRAYON_ELASTICITY .88

/**
 * This struct holds everything needed in the preparation and storage of 
 * a cpBody. It will contain a "point array", two identically-indexed 
 * arrays of x- and y-values for the vertices, but only during creation.
 * After creation, it maintains the color of the body for graphics.
 */
typedef struct body_data {
    GArray *x_values, *y_values;
    long color_rgb;
} Body_data;

typedef struct shape_data Shape_data;

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
void drawing_activate(cpSpace *space, cpBody *drawing, long in_color);

/**
 * Frees the body and all of its shapes
 * Also frees the Point_array in the user data
 *
 * @param cpBody *body to be freed
 */
void free_body_full(cpBody *body);

#endif
