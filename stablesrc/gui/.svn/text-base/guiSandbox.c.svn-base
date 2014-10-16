#include <gtk/gtk.h>
// #include <chipmunk/chipmunk.h>
#include "../../../chipmunk/chipmunk.h"
#include "../graphics/graphics.h"
#include "../core/physics.h"
#include "gui.h"
  
int initialize_window(Gui *gui);   
int initialize_menu(GtkWidget *grid);
int initialize_draw_area(GtkWidget *grid, GtkWidget *draw_area);
int connect_signals(Gui *gui);
static gboolean draw_cb(GtkWidget *draw_area, cairo_t *cr, Gui *gui);     
static gboolean time_handler(Gui *gui); 
static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui); 
static gboolean button_release_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui);
static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, Gui *gui);
     
int main (int argc, char *argv[]){
  Gui *gui = (Gui *)malloc(sizeof(Gui));;
  gui->space = cpSpaceNew();
  cpSpaceSetGravity(gui->space, cpv(0, GRAVITY));
  cpEnableSegmentToSegmentCollisions();
  
  gtk_init(&argc, &argv); 

  if (!initialize_window(gui)){
      return -1;
  }
  
  gtk_main();
  
  cpSpaceFree(gui->space);
  free(gui);
  return 0;
}

int initialize_window(Gui *gui){
  //making the window
  gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(gui->window), "Crayon Physics A++");
  gtk_window_set_default_size(GTK_WINDOW(gui->window), WINDOW_WIDTH, WINDOW_HEIGHT);
  gtk_window_set_position(GTK_WINDOW(gui->window), GTK_WIN_POS_CENTER);
  
  //making the grid to go in the window
  GtkWidget *grid;
  grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(gui->window), grid);
  
  //make the menu
  if (!initialize_menu(grid)){
    return -1;
  }
  
  gui->draw_area = gtk_drawing_area_new();
  initialize_draw_area(grid, gui->draw_area);
  
  gtk_widget_show_all(gui->window);
  cpShape *ground = cpSpaceAddShape(gui->space, cpSegmentShapeNew(gui->space->staticBody, cpv(1, 1), cpv(10, 1), 0));
  cpShapeSetFriction(ground, 0.5);

  gtk_widget_add_events(gui->draw_area, GDK_BUTTON_PRESS_MASK);
  connect_signals(gui);
  
  return 1;
}

int initialize_menu(GtkWidget *grid){
 //making the menu bar
  GtkWidget *menu_box;
  menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_grid_attach(GTK_GRID(grid), menu_box, 0, 0, 1, 1);

  //Making the menu bar
  GtkWidget *menu_bar;
  GtkWidget *file_menu;
  GtkWidget *level_menu;

  //tabs for file menu
  GtkWidget *file;
  GtkWidget *quit;

  //tabs for level menu
  GtkWidget *level;
  GtkWidget *load;

  //making the menubar and the filemenu
  menu_bar = gtk_menu_bar_new();
  file_menu = gtk_menu_new();
  level_menu = gtk_menu_new();

  //Making the menu items for file
  file = gtk_menu_item_new_with_label("File");
  quit = gtk_menu_item_new_with_label("Quit");

  //Making the menu items for level
  level = gtk_menu_item_new_with_label("Level");
  load = gtk_menu_item_new_with_label("Load");

  //setting up submenus
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), file_menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(level), level_menu);

  //appending submenus
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(level_menu), load);

  //appending menus to menu_bar
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), level);

  //packing menu_bar into the menu box
  gtk_box_pack_start(GTK_BOX(menu_box), menu_bar, FALSE, FALSE, 0);
  
  //connecting signals
  g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
   
  //Will be connect to level load function somehow
  //g_signal_connect(G_object(load), "activate", G_CALLBACK(level_load), NULL);
    return 1;
}

int initialize_draw_area(GtkWidget *grid, GtkWidget *draw_area){
  GtkWidget *frame;
    
  //making a frame to hold the draw area
  frame = gtk_frame_new(NULL);
  gtk_grid_attach(GTK_GRID(grid), frame, 0, 1, 1, 1);

  //making the drawing area
  gtk_widget_set_size_request (draw_area, WINDOW_WIDTH, WINDOW_HEIGHT);
  gtk_container_add(GTK_CONTAINER(frame), draw_area);
  
  gtk_widget_set_events (draw_area, gtk_widget_get_events (draw_area)| GDK_BUTTON_PRESS_MASK|GDK_POINTER_MOTION_MASK|GDK_BUTTON_RELEASE_MASK);


  return 0;
}

int connect_signals(Gui *gui){
  //connecting signals for X and menu choices
  g_signal_connect(G_OBJECT(gui->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Signals used to handle the backing surface */
  g_signal_connect (gui->draw_area, "draw", G_CALLBACK (draw_cb), gui);

  //Sets up timer
  g_timeout_add(1000*TIMESTEP, (GSourceFunc) time_handler, (gpointer) gui);

  //mouse click event
  g_signal_connect (gui->draw_area, "button-press-event", G_CALLBACK (button_press_event_cb), gui);

  //mouse release event
  g_signal_connect (gui->draw_area, "button-release-event", G_CALLBACK (button_release_event_cb), gui);

   g_signal_connect (gui->draw_area, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), gui);

  return 0;
}

static gboolean draw_cb(GtkWidget *draw_area, cairo_t *cr, Gui *gui){
  cr = gdk_cairo_create(gtk_widget_get_window(draw_area));
  
  cairo_set_source_rgb (cr, 255, 255, 255);
  cairo_paint (cr);
  cairo_matrix_t *matrix = calloc(1, sizeof(cairo_matrix_t));
  cairo_matrix_init_translate(matrix, 0, WINDOW_HEIGHT);
  double factor = 1.0 / (float) PIXELS_PER_METER;
  cairo_matrix_scale(matrix, factor, factor);
  //draw space
  cairo_set_matrix(cr, matrix);  
  //fprintf(stdout, "Matrix:\t%2.2f  %2.2f\n\t\t\t%2.2f  %2.2f\t\t%2.2f, %2.2f", matrix->xx, matrix->yx, matrix->xy, matrix->yy, matrix->x0, matrix->y0);
  graphics_space_display(gui->draw_area, cr, gui->space);
  free(matrix);
  cairo_destroy(cr);

  return FALSE;
}

static gboolean time_handler(Gui *gui){
  //step through physics and request redraw
  gtk_widget_queue_draw(gui->window);
  cpSpaceStep(gui->space, TIMESTEP);
  
  
  return TRUE;
}

static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui){
  fprintf(stdout, "Mouse clicked:\n");
  fflush(stdout);

  if (event->button == 1) { 
      float x = (float)event->x / (float)PIXELS_PER_METER;
      float y = (float)(-1* event->y + WINDOW_HEIGHT) / (float)PIXELS_PER_METER ;
    
      fprintf(stdout, "x=%2.2f y=%2.2f\n", x, y);
 	
 	//Making new body
 	gui->current_body = drawing_new((cpFloat) x, (cpFloat) y, 0);
  }
 

  return TRUE;
}

static gboolean button_release_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui){
	fprintf(stdout, "Mouse released:\n");
	
	if (event->button == 1){
		drawing_activate(gui->space, gui->current_body);
		fprintf(stdout, "left released:\n");
		
	}
	
	return TRUE;
}

static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, Gui *gui){
  if (event->state & GDK_BUTTON1_MASK){
  	//fprintf(stdout, "MOUSE DRAG\n");
  	float x = (float)event->x / (float)PIXELS_PER_METER;
    float y = (float)(-event->y + WINDOW_HEIGHT) / (float)PIXELS_PER_METER ;
    //fprintf(stdout, "cpBody is sleeping:\t%d", cpBodyIsSleeping(gui->current_body));
    //fprintf(stdout, "x=%2.2f y=%2.2f\n", x, y);
    gui->current_body = drawing_update(gui->space, gui->current_body, x, y);
  }	
  	
  return TRUE;
}
