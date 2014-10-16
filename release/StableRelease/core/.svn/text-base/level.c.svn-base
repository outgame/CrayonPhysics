#include "level.h"

/**
 * Loads up a given level file and adds objects to the given space sp accordingly.
 *
 *
 * First, level_load finds the first list within the JSON object, then it takes in the string representing the list of images of the form "["FILENAME", pos_x, pos_y, width, height],...,["FILENAME", pos_x, pos_y, width, height]" and passes each []-enclosed section to image_parse. Similarly, it then calls obj_parse() on each member of the list of objects in 'dictionary' form: "{"Type":"[]","Args":"[]"} and then save the soundfile as a string. Then wrap everything into a Level and return a pointer to that struct.
 */

/*
static Image * image_parse(char *imagestring);
static Obj * obj_parse(char * objstring);
*/
Level * level_load(cpSpace * sp, char * filename) {
  Level *level = (Level *)malloc(sizeof(Level));

  FILE *ifile = fopen(filename, "r");

  if (ifile != NULL){
    fprintf(stdout, "\n\n%s opened!\n\n", filename);
  }else{
    fprintf(stdout, "\n\n%s failed to open.\n\n", filename);
  }

  fclose(ifile);

  return level;
}


/*
static Image * image_parse(char *imagestring) {
}

static Obj * obj_parse(char * objstring) {

}
*/
