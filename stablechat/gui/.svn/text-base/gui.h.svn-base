#ifndef GUI_HEADER
#define GUI_HEADER

#include <gtk/gtk.h>
#include <chipmunk/chipmunk.h>
#include "../graphics/graphics.h"
#include "../core/physics.h"
#include "../core/level.h"
#include <pthread.h>

#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 500
#define TIMESTEP 1.0/60.0
#define PIXELS_PER_METER 100 
#define GRAVITY -10
#define RED 16711680
#define GREEN 65280
#define BLUE 255
#define BLACK 0
#define RANDOM -1

/**
 *Struct that holds references and variables for all important GUI features
 */
typedef struct gui{
  GtkWidget *window;
  GtkWidget *draw_area;
  GtkWidget *entry;
  GtkWidget *text_view;
  char buff[256];
  pthread_mutex_t buffer_mutex;
  GtkTextMark *mark;
  cpSpace *space;
  cpBody *current_body;
  long current_color;
  int sockfd;
}Gui;

/**
 *Struct that holds the gui and two GtkEntry's for updating the server
 */
typedef struct server_entry{
  Gui *gui;
  GtkWidget *server_entry;
  GtkWidget *port_entry;
}Server;

#endif

/**
 *Function that fress the gui and all associated GUI components
 */
void free_memory(Gui *gui);

