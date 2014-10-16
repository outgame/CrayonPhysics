#include "../gui/gui.h"
#include "graphics.h"
#include "../core/physics.h"
//Body_data from physics.h

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
	//cr = gdk_cairo_create(gtk_widget_get_window(widget));
	cairo_translate(cr, 0, 500);
	cairo_scale(cr, PIXELS_PER_METER, -1.0*PIXELS_PER_METER);

	cairo_set_line_width(cr, CRAYON_RADIUS*1.5);
	cairo_set_source_rgb(cr, 1, 0, 1);
	draw_static_bodies(cr, space, rogue);
	//cairo_translate(cr, 0, 0);
	//cairo_scale(cr, 10, 10);
	//cairo_paint(cr);
	
	cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc) each_body, cr);
	//cairo_matrix_t *matrix = calloc(1, sizeof(cairo_matrix_t));
	//cairo_get_matrix(cr, matrix);
	//fprintf(stdout, "The matrix:\n%5.2f, %5.2f\n%5.2f, %5.2f\n", matrix->xx, matrix->yx, matrix->xy, matrix->yy);
	//fprintf(stdout, "Translations: x=%5.2f, y=%5.2f", matrix->x0, matrix->y0);
	//free(matrix);
	//cairo_arc (cr,1, 2, .5, 0, 2 * G_PI);
	//cairo_stroke(cr);
	//cairo_fill(cr);
	//fprintf(stdout, "That worked.\n");
	//cpBody * statBody = space->staticBody;
	//fprintf(stdout, "That worked\n");
	//each_body(statBody, cr);
}

static void draw_static_bodies(cairo_t *cr, cpSpace *space, cpBody * rogue) {
	cpBody * statBody = space->staticBody;
	//cpVect position = cpBodyLocal2World(statBody, cpBodyGetPos(statBody));
	/*cpVect vA = cpvadd(position, cpSegmentShapeGetA(shape));
	cpVect vB = cpvadd(position, cpSegmentShapeGetB(shape));
	cairo_move_to(cr, vA.x, vA.y);
	cairo_line_to(cr, vB.x, vB.y);
	*/
	if (rogue)
		cpBodyEachShape(rogue, (cpBodyShapeIteratorFunc) each_rogue_shape, cr);
	cpBodyEachShape(statBody, (cpBodyShapeIteratorFunc) each_shape, cr);
	//cpVect position = cpBodyGetPos(statBody);
	//fprintf(stdout, "STATIC:\t");
	//graphics_draw_com(statBody, cr);
	if (rogue) {
		//position = cpBodyGetPos(rogue);
		
		//fprintf(stdout, "ROGUE:\t");
		graphics_draw_com(rogue, cr);
	}
	
	cairo_stroke(cr);
}

void graphics_level_load( Level *lvl) {
	
}

void graphics_draw_com( cpBody *body, cairo_t *cr ) {

	cpVect pos = cpBodyGetPos(body);
	//cpFloat x = pos.x;
	//cpFloat y = pos.y;
	//cairo_user_to_device(cr, &x, &y);
	cairo_arc (cr, pos.x, pos.y, .05, 0, 2 * G_PI);
	//fprintf(stdout, "Should have drawn COM at x=%5.2f, y=%5.2f \n", pos.x, pos.y);
	cairo_stroke(cr);
}

/** 
 * To be called in space_display as cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc) each_body, cr)
 *
 */ 
static void each_body( cpBody *body, cairo_t * cr) {
	RGB color;
	
	Body_data * parray = cpBodyGetUserData(body);
	/*if (parray)	{
		cpVect offset = parray->offset;
		printf("OFFSET:\tx=%5.2f, y=%5.2f\n", offset.x, offset.y);
	}
	*/
	/**
	 * Center of Mass is the reference point for all vertices
	 */
	/*cpVect position = cpBodyGetPos(body);
	int length = (int) (parray->x_values->len);
	cpVect prev = cpvzero;
	cpVect curr = cpv(g_array_index((parray->x_values), cpFloat, 0), g_array_index((parray->y_values), cpFloat, 0));
	curr = cpvadd(curr, position);
	//fprintf(stdout, "Coordinates before: (%5.2f, %5.2f)", curr.x, curr.y);
	//cairo_device_to_user(cr, &curr.x, &curr.y);
	//fprintf(stdout, "Coordinates after: (%5.2f, %5.2f)", curr.x, curr.y);
	for (int i = 1; i < length; i++) {
		prev = curr;
		curr = cpv(g_array_index((parray->x_values), cpFloat, i), g_array_index((parray->y_values), cpFloat, i));
		curr = cpvadd(curr, position);
		//cairo_device_to_user(cr, &curr.x, &curr.y);
		cairo_move_to(cr, prev.x, curr.y);
		cairo_line_to(cr, curr.x, curr.y);
		cairo_stroke(cr);
	}
	*/
	rgb_from_int(parray->color_rgb, &color);
	
	cairo_set_source_rgb(cr, color.r, color.g, color.b);

	graphics_draw_com(body, cr);
	//const cpVect rotation = cpBodyGetRot(body);
	//cpFloat angle = cpvtoangle(rotation);
	//cairo_rotate(cr, angle);
	//if (parray != NULL)
	cpBodyEachShape(body, (cpBodyShapeIteratorFunc) each_shape, cr);
}

static void each_rogue_shape(cpBody *body, cpShape *shape, cairo_t *cr) {
	//fprintf(stdout, "DRAWING ROGUE\n\n");
	each_shape(body, shape, cr);

}


/**
 * To be called in each_body as cpBodyEachShape(body, (cpBodyShapeIteratorFunc) each_shape, cr)
 *
 */

static void each_shape(cpBody *body, cpShape *shape, cairo_t *cr) {
	//cpVect offset = cpvzero;
	//cpVect pos = cpBodyGetPos(body);
	//cpVect origin = cpvzero;
	//Body_data * pa = cpBodyGetUserData(body);
	/*if (pa)	{
		//origin = pa->orig;
		offset = pa->offset;
		//printf("OFFSET:\tx=%5.2f, y=%5.2f\n", offset.x, offset.y);
	}*/
	cpVect vA = cpSegmentShapeGetA(shape);
	cpVect vB = cpSegmentShapeGetB(shape);
	//printf("Originally: x1=%5.2f, y1=%5.2f, x2=%5.2f, y2=%5.2f\n", vA.x, vA.y, vB.x, vB.y);
	//printf("x1=%5.2f, y1=%5.2f, x2=%5.2f, y2=%5.2f\n", vA.x, vA.y, vB.x, vB.y);
	//offset = cpBodyLocal2World(body, offset);

	//vA = cpvsub(vA, offset);
	//vB = cpvsub(vB, offset);

	vA = cpBodyLocal2World(body, vA);
	vB = cpBodyLocal2World(body, vB);
		
	//vA = cpvsub(vA, pos);
	//vB = cpvsub(vB, pos);

	cairo_move_to(cr, vA.x, vA.y);
	cairo_line_to(cr, vB.x, vB.y);
	//printf("x1=%5.2f, y1=%5.2f, x2=%5.2f, y2=%5.2f\n\n", vA.x, vA.y, vB.x, vB.y);
	cairo_stroke(cr);
	//cairo_set_source_rgb(cr, 0, 1, 0);
	//cairo_arc (cr, 10, 2, .5, 0, 2 * G_PI);
	//cairo_stroke_preserve(cr);
	//cairo_fill(cr);

}


/**
 * Decodes a long (at least 32 bits with the last 32 bits being a byte each for a, r, g, b) to an rgb value
 *
 */
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
/*
int main(int argc, char * argv[]) {

	RGB color;
	long val = 16777215;

	rgb_from_int(val, &color);

	//printf("Value was %lx\n", val);
	//printf("Red is %f, Green is %f, Blue is %f\n", color.r, color.g, color.b);

	return EXIT_SUCCESS;
}
*/
