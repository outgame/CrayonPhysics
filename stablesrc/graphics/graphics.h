#include <stdio.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <gtk/gtk.h>
//#include "../../../chipmunk/chipmunk.h"
#include <chipmunk/chipmunk.h>
#include "../core/level.h"

// GRAPHICS header file
#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

/**
 * Iterates over the bodies and draws the shapes in the gtk window
 * calls convert_coordinates() to change the coordinate system
 * reads the data field of the body to know which color to use
 * //will eventually call crayon_stroke() to make the shapes look like crayon
 * 
 * @param pointer to a cpSpace
 */
void graphics_space_display( GtkWidget * widget, cairo_t * cr, cpSpace * space, cpBody * rogue );

/**
 * Initialize the level
 *
 * Iterates through the images and loads them to the gtk background
 * Calls display_space to draw the bodies contained in the space
 * Uses a different library to play the sound file
 * 
 * @param struct level that contains a list of images, the cpSpace, and a sound file
 */
//void graphics_level_load( Level *lvl );

/**
 * Draws a dot at the center of mass of a body by calling cpBodyGetPos()
 * Used for testing purposes to make sure the origin of the body is in the right place
 * 
 * @param chipmunk body
 */
void graphics_draw_com( cpBody *body, cairo_t *cr );

#endif
