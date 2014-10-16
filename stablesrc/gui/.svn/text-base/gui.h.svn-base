#ifndef GUI_HEADER
#define GUI_HEADER

#include <gtk/gtk.h>
#include <chipmunk/chipmunk.h>
#include "../graphics/graphics.h"
#include "../core/physics.h"
#include "../core/level.h"

#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 500
#define TIMESTEP 1.0/60.0
#define PIXELS_PER_METER 100 
#define GRAVITY -10

typedef struct gui{
  GtkWidget *window;
  GtkWidget *draw_area;
  GtkWidget *entry;
  GtkWidget *text_view;
  GtkTextMark *mark;
  cpSpace *space;
  cpBody *current_body;
  int sockfd;
}Gui;
	
#endif
