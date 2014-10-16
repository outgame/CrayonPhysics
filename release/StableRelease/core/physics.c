#include "physics.h"

static cpBody *add_segment_to_body(cpSpace *space, cpBody *drawing);
static cpBody *utils_update_drawing(cpBody *drawing);
static Body_data* shift_origin(cpBody * body, cpVect old_origin, cpVect new_origin);
static Body_data *point_array_new(cpFloat x, cpFloat y, long color_rgb); 
static void point_array_free(Body_data *pa); 
static void free_shape(cpBody *body, cpShape *shape, void *data);
static void point_array_free_arrays(Body_data *pa);
static void post_step_body_replace_shapes(cpSpace *space, cpBody *body, void *data);

#ifdef DEBUG

#define TIMESTEP 1.0/40.0
#define GRAVITY -98

static void print_positions(cpBody* drawing) {
    cpVect pos = cpBodyGetPos(drawing);
    printf("Center of mass (%2.2f, %2.2f)\n", pos.x, pos.y);
    Body_data *pa = (Body_data *) cpBodyGetUserData(drawing);
    for (int i = 0; i < pa->x_values->len; i++) {
        printf("Segment %d starts at (%2.2f, %2.2f)\n", i, g_array_index(pa->x_values, cpFloat, i), g_array_index(pa->y_values, cpFloat, i)); 
    }
}

int main (int argc, char **argv) {
    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, GRAVITY));
    cpEnableSegmentToSegmentCollisions();
    cpShape *ground = cpSpaceAddShape(space, cpSegmentShapeNew(space->staticBody, cpv(0, 5), cpv(10, -5), 0));
    cpShapeSetFriction(ground, 0.3);
    cpBody *drawing = drawing_new(2, 7, 0);
    drawing = drawing_update(space, drawing, 3, 8);
    drawing = drawing_update(space, drawing, 3, 9);
    drawing = drawing_update(space, drawing, 2, 9);
    print_positions(drawing);
    cpVect vel, pos;
    int i = 0;
    for(cpFloat time = 0; time < 2; time += TIMESTEP){
        pos = cpBodyGetPos(drawing);
        vel = cpBodyGetVel(drawing);
        printf( "Time = %2.2f Position = (%2.2f, %2.2f) Velocity = (%2.2f, %2.2f)\n",
            time, pos.x, pos.y, vel.x, vel.y);
        cpSpaceStep(space, TIMESTEP);
        i++;
        if (i == 20) {
            drawing_activate(space, drawing);
            print_positions(drawing);
        }
    }
    print_positions(drawing);
    free_body_full(drawing);
    cpShapeFree(ground);
    cpSpaceFree(space);
    return EXIT_SUCCESS;
}

#endif


Body_data *point_array_new(cpFloat x, cpFloat y, long color_rgb) {
    Body_data *pa = (Body_data *) calloc( 1, sizeof(Body_data) );
    pa->color_rgb = color_rgb;

    pa->x_values = g_array_sized_new(FALSE, FALSE, sizeof(cpFloat), 4);
    pa->y_values = g_array_sized_new(FALSE, FALSE, sizeof(cpFloat), 4);

    g_array_append_val(pa->x_values, x);
    g_array_append_val(pa->y_values, y);

    return pa;
}
void point_array_free(Body_data *pa) {
    free(pa);
}
static void point_array_free_arrays(Body_data *pa) {
    g_array_free(pa->x_values, TRUE);
    g_array_free(pa->y_values, TRUE);
}
cpBody* drawing_new(cpFloat x, cpFloat y, long color_rgb) {
    cpBody *drawing = cpBodyNew(INITIAL, INITIAL);
	cpVect clickpoint = cpv(x, y);
    cpBodySetPos(drawing, clickpoint);
    //cpVect position = cpBodyGetPos(drawing);
	//fprintf(stdout, "INITIAL POSITION AT: (%5.2f, %5.2f)\n\n", position.x, position.y);
    Body_data *pa = point_array_new(0, 0, color_rgb);
    cpBodySetUserData(drawing, pa);
    return drawing;
}
cpBody* drawing_update(cpSpace *space, cpBody *drawing, cpFloat x, cpFloat y) {
    Body_data *pa = cpBodyGetUserData(drawing);
    int length = pa->x_values->len;
	cpVect old = cpvzero;
    old.x = g_array_index(pa->x_values, cpFloat, length - 1);
	old.y = g_array_index(pa->y_values, cpFloat, length - 1);
	old = cpBodyLocal2World(drawing, old);
	cpVect point = cpv(x, y);
    if (cpvdistsq(old, point) > THRESHOLD) {
		cpVect position = cpBodyGetPos(drawing);
		point = cpvsub(point, position);
		//fprintf(stdout, "Point= (%5.2f, %5.2f) while x= %5.2f, y=%5.2f\n", point.x, point.y, x, y);
		x = point.x;
		y = point.y;
		    g_array_append_val(pa->x_values, x);
		    g_array_append_val(pa->y_values, y);
		    cpBodySetUserData(drawing, pa);
		    drawing = add_segment_to_body(space, drawing);
    }
    cpBodySetVel(drawing, cpvzero);
    return drawing;
}
static cpBody *add_segment_to_body(cpSpace *space, cpBody *drawing) {
    Body_data *pa = cpBodyGetUserData(drawing);
    int length = pa->x_values->len;
    cpShape *seg = cpSegmentShapeNew(drawing,
                            cpv(g_array_index(pa->x_values, cpFloat, length - 2),
                                g_array_index(pa->y_values, cpFloat, length - 2)),
                            cpv(g_array_index(pa->x_values, cpFloat, length - 1),
                                g_array_index(pa->y_values, cpFloat, length - 1)),
                            CRAYON_RADIUS);
    cpShapeSetFriction(seg, CRAYON_FRICTION);
    cpShapeSetElasticity(seg, CRAYON_ELASTICITY);
    cpSpaceAddShape(space, seg);
    return drawing;
}
void drawing_activate(cpSpace *space, cpBody *drawing, long in_color) {
  long seed = (long int) drawing;
  srand((unsigned int) seed);    
  long color = rand();
  cpSpaceAddBody(space, drawing);
  drawing = utils_update_drawing(drawing);
  Body_data * parray = cpBodyGetUserData(drawing);
  
  if (in_color == -1){
    parray->color_rgb = color;
  }else{
    parray->color_rgb = in_color;  
  }
  
  point_array_free_arrays(parray);
}
static cpBody *utils_update_drawing(cpBody *drawing) {
    cpFloat mass = cpBodyGetMass(drawing);
    cpFloat moment = cpBodyGetMoment(drawing);
    Body_data *pa = cpBodyGetUserData(drawing);
    cpVect pos_a, pos_b;
    cpVect origin = cpBodyGetPos(drawing);
    cpFloat mi, micx = 0, micy = 0;
    int length = pa->x_values->len;
    for (int index = 1; index < length; index++) {
        pos_a = cpv(g_array_index(pa->x_values, cpFloat, index - 1), g_array_index(pa->y_values, cpFloat, index - 1));
        pos_b = cpv(g_array_index(pa->x_values, cpFloat, index), g_array_index(pa->y_values, cpFloat, index));
        mi = (CRAYON_MASS * cpAreaForSegment( pos_a, pos_b, CRAYON_RADIUS ));
        micx += mi * ((pos_a.x + pos_b.x) / 2);
        micy += mi * ((pos_a.y + pos_b.y) / 2);
        mass += mi;
        moment += cpMomentForSegment(mass, pos_a, pos_b); 
    }
    cpBodySetMass(drawing, mass);
    cpBodySetMoment(drawing, moment);
    cpVect new_origin = cpv(micx / mass, micy / mass);
    new_origin = cpBodyLocal2World(drawing, new_origin);
    cpBodySetPos( drawing, new_origin );
    cpSpace * space = cpBodyGetSpace(drawing);
	cpSpaceReindexShapesForBody(space, drawing);
    pa = shift_origin(drawing, origin, new_origin);
    cpBodySetUserData(drawing, pa);
	if (space)
		post_step_body_replace_shapes(space, drawing, NULL);
	else
		fprintf(stderr, "WTF\n");
    return drawing;
}
static Body_data* shift_origin(cpBody *body, cpVect old_origin, cpVect new_origin) {
	Body_data *pa = cpBodyGetUserData(body);
    cpVect offset = cpvsub(new_origin, old_origin);
    cpFloat x_offset = offset.x;
    cpFloat y_offset = offset.y;
    int length = pa->x_values->len;
    cpFloat x, y;
    GArray *new_x = g_array_sized_new(FALSE, FALSE, sizeof(cpFloat), length);
    GArray *new_y = g_array_sized_new(FALSE, FALSE, sizeof(cpFloat), length);
    for (int index = 0; index < length; index++) {
        x = g_array_index(pa->x_values, cpFloat, index) - x_offset;
        y = g_array_index(pa->y_values, cpFloat, index) - y_offset;
        g_array_append_val( new_x, x );
        g_array_append_val( new_y, y );
    }
    g_array_free(pa->x_values, TRUE);
    g_array_free(pa->y_values, TRUE);
    pa->x_values = new_x;
    pa->y_values = new_y;
    return pa;
}
static void post_step_body_replace_shapes(cpSpace *space, cpBody *body, void *data) {
	Body_data *pa = cpBodyGetUserData(body);
    int length = pa->x_values->len;
	cpBodyEachShape(body, (cpBodyShapeIteratorFunc) free_shape, NULL);
	cpSpaceReindexShapesForBody(space, body);
	for (int index = 1; index < length; index++) {
		cpFloat x = g_array_index(pa->x_values, cpFloat, index);
        cpFloat y = g_array_index(pa->y_values, cpFloat, index);
		cpShape *seg = cpSegmentShapeNew(body, cpv(x,y), cpv(g_array_index(pa->x_values, cpFloat, index - 1), g_array_index(pa->y_values, cpFloat, index - 1)), CRAYON_RADIUS);
		cpShapeSetFriction(seg, CRAYON_FRICTION);
		cpSpaceAddShape(space, seg);
	}
	cpSpaceReindexShapesForBody(space, body);
}
void free_body_full(cpBody *body) {
    cpBodyEachShape(body, (cpBodyShapeIteratorFunc) free_shape, NULL);
    point_array_free( (Body_data *) cpBodyGetUserData(body) );
	cpSpace *space = cpBodyGetSpace(body);
	cpSpaceRemoveBody(space, body);
	cpBodyDestroy(body);
    cpBodyFree(body);
}
static void free_shape(cpBody *body, cpShape *shape, void *data) {
	cpSpace *space = cpBodyGetSpace(body);
	cpSpaceRemoveShape(space, shape);
	cpShapeDestroy(shape);
    cpShapeFree(shape);
}
