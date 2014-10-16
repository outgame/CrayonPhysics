#include "../gui/gui.h"
#include "graphics.h"
#include "../core/core.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct rgb_value {
	double r;
	double g;
	double b;
} RGB;

static void each_body( cpBody *body, cairo_t *cr);
static void each_shape(cpBody *body, cpShape *shape, cairo_t *cr);
static double get_color(long *pLval);
static void rgb_from_int(long lVal, RGB * col); 
static void draw_static_bodies(cairo_t *cr, cpSpace *space, cpBody * rogue);
static void each_rogue_shape(cpBody *body, cpShape *shape, cairo_t *cr);


void graphics_space_display( GtkWidget * widget, cairo_t * cr, cpSpace *space, cpBody * rogue) {
	cairo_translate(cr, 0, 500);
	cairo_scale(cr, PIXELS_PER_METER, -1.0*PIXELS_PER_METER);
	cairo_set_line_width(cr, CRAYON_RADIUS*1.5);
	cairo_set_source_rgb(cr, 0, .5, .24);

	draw_static_bodies(cr, space, rogue);

	cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc) each_body, cr);
}


static void draw_static_bodies(cairo_t *cr, cpSpace *space, cpBody * rogue) {
	cpBody * statBody = space->staticBody;

	if (rogue)
		cpBodyEachShape(rogue, (cpBodyShapeIteratorFunc) each_rogue_shape, cr);

	cpBodyEachShape(statBody, (cpBodyShapeIteratorFunc) each_shape, cr);

	if (rogue) {
		graphics_draw_com(rogue, cr);
	}

	cairo_stroke(cr);
}


void graphics_draw_com( cpBody *body, cairo_t *cr ) {
	cpVect pos = cpBodyGetPos(body);

	cairo_arc (cr, pos.x, pos.y, .03, 0, 2 * G_PI);
	cairo_stroke(cr);
}


static void each_body( cpBody *body, cairo_t * cr) {
	RGB color;

	Body_data * parray = cpBodyGetUserData(body);

	rgb_from_int(parray->color_rgb, &color);
	cairo_set_source_rgb(cr, color.r, color.g, color.b);

	graphics_draw_com(body, cr);
	cpBodyEachShape(body, (cpBodyShapeIteratorFunc) each_shape, cr);
}


static void each_rogue_shape(cpBody *body, cpShape *shape, cairo_t *cr) {
	each_shape(body, shape, cr);
}


static void each_shape(cpBody *body, cpShape *shape, cairo_t *cr) {
	cpVect vA = cpSegmentShapeGetA(shape);
	cpVect vB = cpSegmentShapeGetB(shape);

	vA = cpBodyLocal2World(body, vA);
	vB = cpBodyLocal2World(body, vB);

	cairo_move_to(cr, vA.x, vA.y);
	cairo_line_to(cr, vB.x, vB.y);
	cairo_stroke(cr);
}


static void rgb_from_int(long lVal, RGB * col) {
	(col->b) = get_color(&lVal);
	(col->g) = get_color(&lVal);
	(col->r) = get_color(&lVal);
}


static double get_color(long *pLval) {
	long result = *pLval & 255;
	*pLval >>= 8;
	return (double) (result / 255.0);
}


