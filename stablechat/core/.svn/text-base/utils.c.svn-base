/**
 * \file utils.c
 * \brief Core utilities for client storage and manipulation of bodies.
 *
 * This file will contain functions necessary for the protocol for 
 * non-chat messages between server and clients. 
 *
 */
#include "utils.h"
static long cosine(int hund_degrees);
static long sine(int hund_degrees);

void point_rotate_point(Point * p, int hund_degrees) {
	;
}

/**
 * Returns 10000 times the cosine of the given angle given in degrees * 100
 */
static long cosine(int hund_degrees) {
	long PI10_9 = 3141592654;
	return 0;
}

static long sine(int hund_degrees) {
	return cosine(hund_degrees - 9000);
}

Space * space_new() {
	Space * sp = malloc(sizeof(Space));
	sp->bodies = g_array_new(FALSE, FALSE, sizeof(Body));
	return sp;
}

void space_add_body(Space *space, Body *body) {
	g_array_append_val(space->bodies, body);
}

Body * body_new(int *position, int rotation, long color_rgb) {
	Body * b = malloc(sizeof(Body));

	b->x_values = g_array_sized_new(FALSE, FALSE, sizeof(int), 10);
	b->y_values = g_array_sized_new(FALSE, FALSE, sizeof(int), 10);

	b->shapes = g_array_new(FALSE, FALSE, sizeof(Shape));

	b->color_rgb = color_rgb;
	b->position = position;
	b->rotation = rotation;

	return b;
}

void body_add_shape(Body * body, Shape *shape) {
	g_array_append_val(body->shapes, shape);
}

Shape * shape_new(Body *body, int type, Point *points, int length, int *dashes) {
	Shape * sh = (Shape *) malloc(sizeof(Shape));
	
	sh->type = type;
	sh->x_values = g_array_new(FALSE, FALSE, sizeof(int));
	sh->y_values = g_array_new(FALSE, FALSE, sizeof(int));
	
	for(int i = 0; i < length; i++) {
		int x = points[0].x;
		int y = points[0].y;

		g_array_append_val(sh->x_values, x);
		g_array_append_val(sh->y_values, y);
	}

	sh->dashes = dashes;
	
	return sh;
}

void shape_free(Space *sp, Body *b, Shape *sh) {
	free(sh->x_values);
	free(sh->y_values);
}

int main(int argc, char **argv) {
	return EXIT_SUCCESS;
}

