
#ifndef LEVEL_HEADER
#define LEVEL_HEADER

//#include "../../../chipmunk/chipmunk.h"
#include <chipmunk/chipmunk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** \file level.h
 *  \brief Contains everything needed to load a level
 *
 *
 */

/**
 * Packages the filename of an object, its initial position, x and y, and dimensions, width and height.
 */
typedef struct our_image {
	int x, y;
	int width, height;
	char * filename[];

} Image;

/**
 * Packages sound, pre-loaded images and a chipmunk space of bodies
 */
typedef struct level {
	Image * images;
	cpSpace * sp;
	char * soundfile;
} Level;

/**
 * Contains whatever arguments we decide constitute an "object"
 */
typedef struct our_object {
	int type;
	int vertices;
} Obj;

/**
 * Returns a level struct containing everything needed to load
 */
Level * level_load(cpSpace * sp, char * filename);

void load_level(cpSpace *sp);

#endif
