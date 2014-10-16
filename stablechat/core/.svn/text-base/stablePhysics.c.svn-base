#include "physics.h"

static cpBody *add_segment_to_body(cpSpace *space, cpBody *drawing);
static cpBody *utils_update_drawing(cpBody *drawing);
static Point_array* shift_origin(Point_array *pa, cpVect old_origin, cpVect new_origin);

static Point_array *point_array_new(cpFloat x, cpFloat y, long color_rgb); // constuctor
static void point_array_free(Point_array *pa); // destructor

static void free_shape(cpBody *body, cpShape *shape, void *data);

static void point_array_free_arrays(Point_array *pa);// partial destructor
#ifdef DEBUG

#define TIMESTEP 1.0/40.0
#define GRAVITY -98

static void print_positions(cpBody* drawing) {
    cpVect pos = cpBodyGetPos(drawing);
    printf("Center of mass (%2.2f, %2.2f)\n", pos.x, pos.y);
    
    Point_array *pa = (Point_array *) cpBodyGetUserData(drawing);
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
    //cpBody *drawing = drawing_new(0, 0, 0);
    //drawing = drawing_update(space, drawing, 2, 0);
    //drawing = drawing_update(space, drawing, 2, 2);
    //drawing = drawing_update(space, drawing, 0, 2);
    print_positions(drawing);
    
    //drawing_activate(drawing);
    //print_positions(drawing);
    
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

// Constructor
Point_array *point_array_new(cpFloat x, cpFloat y, long color_rgb) {
    Point_array *pa = (Point_array *) malloc( sizeof(Point_array) );
    pa->color_rgb = color_rgb;
    
    pa->x_values = g_array_new(FALSE, FALSE, sizeof(cpFloat));
    pa->y_values = g_array_new(FALSE, FALSE, sizeof(cpFloat));
    
    g_array_append_val(pa->x_values, x);
    g_array_append_val(pa->y_values, y);
    
    return pa;
}

// Partial Destructors
void point_array_free(Point_array *pa) {
    free(pa);
}

static void point_array_free_arrays(Point_array *pa) {
    g_array_free(pa->x_values, TRUE);
    g_array_free(pa->y_values, TRUE);
}

cpBody* drawing_new(cpFloat x, cpFloat y, long color_rgb) {
    cpBody *drawing = cpBodyNew(INITIAL, INITIAL);
    cpBodySetPos(drawing, cpv(x, y));
    
    Point_array *pa = point_array_new(x, y, color_rgb);
    cpBodySetUserData(drawing, pa);
    
    return drawing;
}

cpBody* drawing_update(cpSpace *space, cpBody *drawing, cpFloat x, cpFloat y) {
    Point_array *pa = cpBodyGetUserData(drawing);
    int length = pa->x_values->len;
    
    // if the difference between the new x and the previous x is greater than the threshold
    if (( abs((SCALE * x) - (SCALE * g_array_index(pa->x_values, cpFloat, length - 1))) >= (SCALE * THRESHOLD) ) ||
        ( abs((SCALE * y) - (SCALE * g_array_index(pa->y_values, cpFloat, length - 1))) >= (SCALE * THRESHOLD) )) {
        cpVect point = cpv(x, y);
	cpBodyWorld2Local(drawing, point);
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

// once a new point has been added to the point_array, add the new segment to the body
static cpBody *add_segment_to_body(cpSpace *space, cpBody *drawing) {
    Point_array *pa = cpBodyGetUserData(drawing);
    int length = pa->x_values->len;
    
    cpShape *seg = cpSegmentShapeNew(drawing,
                            cpv(g_array_index(pa->x_values, cpFloat, length - 2),
                                g_array_index(pa->y_values, cpFloat, length - 2)),
                            cpv(g_array_index(pa->x_values, cpFloat, length - 1),
                                g_array_index(pa->y_values, cpFloat, length - 1)),
                            CRAYON_RADIUS);
    
    cpShapeSetFriction(seg, CRAYON_FRICTION);
    //cpShapeSetElasticity(cpShape *shape, cpFloat value)?
    cpSpaceAddShape(space, seg);
    
    return drawing;
}

void drawing_activate(cpSpace *space, cpBody *drawing) {
    drawing = utils_update_drawing(drawing);
    
    cpSpaceAddBody(space, drawing);
    Point_array * parray = cpBodyGetUserData(drawing);
    point_array_free_arrays(parray);
}

static cpBody *utils_update_drawing(cpBody *drawing) {
    cpFloat mass = cpBodyGetMass(drawing);
    cpFloat moment = cpBodyGetMoment(drawing);
    
    Point_array *pa = cpBodyGetUserData(drawing);
    cpFloat x = g_array_index(pa->x_values, cpFloat, 0);
    cpFloat y = g_array_index(pa->y_values, cpFloat, 0);
    
    cpVect pos_a, pos_b;
    cpVect origin = cpBodyGetPos(drawing);
    
    cpFloat mi, micx = 0, micy = 0;
    int length = pa->x_values->len;
    for (int index = 1; index < length; index++) {
        pos_a = cpv(g_array_index(pa->x_values, cpFloat, index - 1), g_array_index(pa->y_values, cpFloat, index - 1));
        pos_b = cpv(g_array_index(pa->x_values, cpFloat, index), g_array_index(pa->y_values, cpFloat, index));
 	cpvadd(pos_a, origin);
	cpvadd(pos_b, origin);       
        x += pos_b.x;
        y += pos_b.y;
        
        mi = (CRAYON_MASS * cpAreaForSegment( pos_a, pos_b, CRAYON_RADIUS ));
        micx += mi * ((pos_a.x + pos_b.x) / 2);
        micy += mi * ((pos_a.y + pos_b.y) / 2);
        
        mass += mi;
        moment += cpMomentForSegment(mass, pos_a, pos_b); // not actually sum
    }

    cpBodySetMass(drawing, mass);
    cpBodySetMoment(drawing, moment);
    
    // center of mass is the average of all vertices  NOT
    //cpVect new_origin = cpv(x / length, y / length);
    cpVect new_origin = cpv(micx / mass, micy / mass);
    cpBodySetPos( drawing, new_origin );
    //cpBodySetPos(drawing, cpv(pos.x + (second.x / length), pos.y + (second.y / length)));
    
    pa = shift_origin(pa, origin, new_origin);
    cpBodySetUserData(drawing, pa);
    
    return drawing;
}

static Point_array* shift_origin(Point_array *pa, cpVect old_origin, cpVect new_origin) {
    pa->orig = new_origin;
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

void free_body_full(cpBody *body) {
    cpBodyEachShape(body, (cpBodyShapeIteratorFunc) free_shape, NULL);
    
    point_array_free( (Point_array *) cpBodyGetUserData(body) );
    cpBodyFree(body);
}

static void free_shape(cpBody *body, cpShape *shape, void *data) {
    cpShapeFree(shape);
}


