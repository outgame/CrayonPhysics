/**
 * \file utils.h
 * \brief Core utilities for client storage and manipulation of bodies.
 *
 * This file will govern the protocol for non-chat messages between server
 * and clients. The structs defined here remove the need for Chipmunk on
 * client installations.
 */
#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

typedef struct intpair {
	int x, y;
} Point;

/**
 * Holds everything needed to draw the space. bodies holds pointers to 
 * Body objects, where index 0 always refers to a rogue body-in-progress,
 * meaning it can have new elements added.
 */
typedef struct space_sans_chipmunk {
    	GArray *bodies;
} Space;

/**
 * Holds everything needed to draw the body.
 */
typedef struct body_sans_chipmunk {
    	GArray *x_values;
    	GArray *y_values;
    	GArray *shapes;
	long color_rgb;
	int *position;
	int rotation;
} Body;

/**
 * Holds everything needed to draw a shape. type will index into an array
 * of function pointers in graphics to draw circles, polygons, or others.
 */
typedef struct shape_sans_chipmunk {
	int type;
	GArray *x_values;
	GArray *y_values;
	int *dashes;
} Shape;

Space* space_new(); // level load
void space_add_body(Space *space, Body *body); // mouse release

Body* body_new(int *position, int rotation, long color_rgb); // mouse press
void body_add_shape(Body * body, Shape *shape); // mouse drag
//void body_activate(Body *body); // mouse release
//void body_update_pos(Body *body, double *position, double rotation); // physics time step

Shape* shape_new(Body *body, int type, Point *points, int length, int *dashes); // mouse drag

void shape_free(Space *sp, Body *b, Shape *sh);

void body_free(Space *sp, Body *b);

void space_free(Space *sp);

typedef void (*BodyShapeIteratorFunc)(Body * body, Shape * shape, void *data);

void BodyEachShape(Body *body, BodyShapeIteratorFunc func, void *data);

typedef void (*SpaceBodyIteratorFunc)(Space * space, Body * body, void *data);

void SpaceEachBody(Space * space, SpaceBodyIteratorFunc func, void *data);

void point_rotate_point(Point * p, int hund_degrees);


#endif
