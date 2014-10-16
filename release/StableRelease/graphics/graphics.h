#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <gtk/gtk.h>
#include <chipmunk/chipmunk.h>

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
 * Draws a dot at the center of mass of a body by calling cpBodyGetPos()
 * Used for testing purposes to make sure the origin of the body is in the right place
 * 
 * @param chipmunk body
 */
void graphics_draw_com( cpBody *body, cairo_t *cr );

#endif
