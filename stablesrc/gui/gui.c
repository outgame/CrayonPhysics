#include "gui.h"
#include "../core/client.h"
#include <pthread.h>
  
int initialize_window(Gui *gui);   
int initialize_menu(GtkWidget *grid, Gui *gui);
int initialize_draw_area(GtkWidget *grid, GtkWidget *draw_area);
int initialize_text_entry(GtkWidget *grid, Gui *gui);
int initialize_text_view(GtkWidget *grid, Gui *gui);
int initialize_client(Gui *gui);
void *client_recv1(void *ptr);
int add_to_text(Gui *gui, gchar *s);
int connect_signals(Gui *gui);
static gboolean send_cb(GtkWidget *widget, Gui *gui);
static gboolean draw_cb(GtkWidget *draw_area, cairo_t *cr, Gui *gui);     
static gboolean time_handler(Gui *gui); 
static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui); 
static gboolean button_release_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui);
static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, Gui *gui);
static gboolean load_cb(Gui *gui);
static gboolean select_cb(void);

int main (int argc, char *argv[]){
  Gui *gui = (Gui *)malloc(sizeof(Gui));;
  gui->space = cpSpaceNew();
  gui->current_body = NULL;
  cpSpaceSetGravity(gui->space, cpv(0, GRAVITY));
  cpEnableSegmentToSegmentCollisions();
  
  gtk_init(&argc, &argv); 

  initialize_window(gui);

  initialize_client(gui);  
  
  pthread_t t1;
  int iret1 = pthread_create(&t1, NULL, client_recv1, gui);

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
  if (!initialize_menu(grid, gui)){
    return -1;
  }
  
  //initializing the draw area
  gui->draw_area = gtk_drawing_area_new();
  initialize_draw_area(grid, gui->draw_area);
  
  //initialize the text entry box
  gui->entry = gtk_entry_new();
  initialize_text_entry(grid, gui);

  //initialize the text view
  gui->text_view = gtk_text_view_new();
  initialize_text_view(grid, gui);

  //shapes and bodies for testing
  gtk_widget_show_all(gui->window);
  cpShape *ground = cpSpaceAddStaticShape(gui->space, cpSegmentShapeNew(gui->space->staticBody, cpv(0, 1), cpv(10, 1), CRAYON_RADIUS * 2));
  cpShapeSetFriction(ground, 1.0);
  cpSpaceAddStaticShape(gui->space, cpSegmentShapeNew(gui->space->staticBody, cpv(3, -100), cpv(3, 100), CRAYON_RADIUS * 2));
  //add events and connect signals
  gtk_widget_add_events(gui->draw_area, GDK_BUTTON_PRESS_MASK);
  connect_signals(gui);
  
  return 1;
}

//initializes the client communication
int initialize_client(Gui *gui){
  char *server = "127.0.0.1";
  int port = 5993;
  gui->sockfd = client_socket_connect(server, port);

  

  return 1;
}

int initialize_menu(GtkWidget *grid, Gui *gui){
 //making the menu bar
  GtkWidget *menu_box;
  menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_grid_attach(GTK_GRID(grid), menu_box, 0, 0, 1, 1);

  //Making the menu bar
  GtkWidget *menu_bar;
  GtkWidget *file_menu;
  GtkWidget *level_menu;
  GtkWidget *server_menu;

  //tabs for file menu
  GtkWidget *file;
  GtkWidget *quit;

  //tabs for level menu
  GtkWidget *level;
  GtkWidget *load;

  //Tabs for server menu
  GtkWidget *server;
  GtkWidget *select;

  //making the menubar and the filemenu
  menu_bar = gtk_menu_bar_new();
  file_menu = gtk_menu_new();
  level_menu = gtk_menu_new();
  server_menu = gtk_menu_new();

  //Making the menu items for file
  file = gtk_menu_item_new_with_label("File");
  quit = gtk_menu_item_new_with_label("Quit");

  //Making the menu items for level
  level = gtk_menu_item_new_with_label("Level");
  load = gtk_menu_item_new_with_label("Load");

  //Making the menu items for server
  server = gtk_menu_item_new_with_label("Server");
  select = gtk_menu_item_new_with_label("Select Server");

  //setting up submenus
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), file_menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(level), level_menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(server), server_menu);

  //appending submenus
  gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(level_menu), load);
  gtk_menu_shell_append(GTK_MENU_SHELL(server_menu), select);

  //appending menus to menu_bar
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), level);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), server);

  //packing menu_bar into the menu box
  gtk_box_pack_start(GTK_BOX(menu_box), menu_bar, FALSE, FALSE, 0);
  
  //connecting signals
  g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(G_OBJECT(load), "activate", G_CALLBACK(load_cb), gui);   
  g_signal_connect(G_OBJECT(select), "activate", G_CALLBACK(select_cb), NULL);
  
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

int initialize_text_entry(GtkWidget *grid, Gui *gui){
  //make the box to pack the button and entry in
  GtkWidget *hbox, *button;
  hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  button = gtk_button_new_with_label("Send");

  //connect button signal
  g_signal_connect(button, "clicked", G_CALLBACK(send_cb), gui);

  //pack the entry and button into the box
  gtk_box_pack_start(GTK_BOX(hbox), gui->entry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

  //pack the box into the grid
  gtk_grid_attach(GTK_GRID(grid), hbox, 0, 2, 1, 1);

  return 0;
}

int initialize_text_view(GtkWidget *grid, Gui *gui){
  //construct the box to put the text view in
  GtkWidget *box;
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_widget_set_size_request(gui->text_view, 250, WINDOW_HEIGHT);

  //don't show cursor and don't allow user to edit the text
  gtk_text_view_set_editable(GTK_TEXT_VIEW(gui->text_view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(gui->text_view), FALSE);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(gui->text_view), GTK_WRAP_WORD_CHAR);
  
  //set header
  GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui->text_view));
  const gchar *header = ">Welcome to Crayon Physics A++!\n>This is a game that allows you to interactively draw shapes to complete a goal. Good luck and have fun.\n";
  
  //later use g_strlcat to add to this string to update buffer
  
  //set the buffer text header
  gtk_text_buffer_set_text(buff, header, -1);
  
  //snap the text view and box into the grid
  gtk_box_pack_start(GTK_BOX(box), gui->text_view, TRUE, TRUE, 0);
  gtk_grid_attach(GTK_GRID(grid), box, 1, 1, 1, 2);
  
  return 0;
}

int add_to_text(Gui *gui, gchar *s){
  //get the iterator and buffer
  GtkTextIter iter;
  GtkTextBuffer *buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui->text_view));
  
  //fprintf(stdout, "Text passed from the entry is %s\n", s);
  
  //add the string to the end of the buffer
  const gchar *string_to_add = s;
  //fprintf(stdout, "%s", string_to_add);
  gtk_text_buffer_get_end_iter(buff, &iter); //sets iterator to the end of the buffer
  gtk_text_buffer_insert(buff, &iter, string_to_add, -1);

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

   //mouse drag event
   g_signal_connect (gui->draw_area, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), gui);

  return 0;
}

static gboolean send_cb(GtkWidget *widget, Gui *gui){
  //get text from entry
  gchar *text = gtk_entry_get_text(GTK_ENTRY(gui->entry));

  //  fprintf(stdout, "Text in the entry is %s\n", text);

  //this is where we send the text, currently just adding it to the text window
  gchar *new_tab = "> ";
  add_to_text(gui, new_tab);
  
  add_to_text(gui, text); 
  client_send(gui->sockfd, text);
  
  gchar *new_line = "\n";
  add_to_text(gui, new_line);

  //clear the text entry box
  const char *clear = "";
  gtk_entry_set_text(GTK_ENTRY(gui->entry), clear);

  return TRUE;
}

static gboolean load_cb(Gui *gui){
  fprintf(stdout, "Load called\n");
  fflush(stdout);  

  //open up the file entry dialog
  GtkWidget *dialog;
  char *filename;

  dialog = gtk_file_chooser_dialog_new("Level Chooser", GTK_WINDOW(gui->window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
  
  //if the user selects a file, get the filename
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT){
	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	fprintf(stdout, "%s\n", filename);
	fflush(stdout);
  }
  
  //call level load using the filename
  level_load(gui->space, filename);

  //destroy the file chooser
  gtk_widget_destroy(dialog);

  return TRUE;
}

static gboolean select_cb(void){
  fprintf(stdout, "Select Server called\n");

  return TRUE;
}

static gboolean draw_cb(GtkWidget *draw_area, cairo_t *cr, Gui *gui){
  //get a cairo instance
  cr = gdk_cairo_create(gtk_widget_get_window(draw_area));
  
  //clear the surface
  cairo_set_source_rgb (cr, 255, 255, 255);
  cairo_paint (cr);
  
  //draw space  
  graphics_space_display(gui->draw_area, cr, gui->space, gui->current_body);
  
  //destroy the cairo instance
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
  //fprintf(stdout, "Mouse clicked:\n");
  fflush(stdout);
  
  //only if left clicked
  if (event->button == 1) { 
    //get the coordinates of the mouse click converted into world coordinates
    float x = (float)event->x / (float)PIXELS_PER_METER;
    float y = (float)(-event->y + WINDOW_HEIGHT) / (float)PIXELS_PER_METER ;
    fprintf(stdout, "x=%2.2f y=%2.2f\n", x, y);
    fflush(stdout);
    
    //Making new body
    gui->current_body = drawing_new((cpFloat) x, (cpFloat) y, 0);
 }
 

  return TRUE;
}

static gboolean button_release_event_cb (GtkWidget *widget, GdkEventButton *event, Gui *gui){
  //fprintf(stdout, "Mouse released:\n");
  
  //only if the left click is released
  if (event->button == 1){
    drawing_activate(gui->space, gui->current_body);
    gui->current_body = NULL;//fprintf(stdout, "left released:\n");
  }
  
  return TRUE;
}

static gboolean motion_notify_event_cb(GtkWidget *widget, GdkEventMotion *event, Gui *gui){
  //only add to the body if it is not NULL
  if ((event->state && GDK_BUTTON1_MASK) && (gui->current_body != NULL)){
    float x = (float)event->x / (float)PIXELS_PER_METER;
    float y = (float)(-event->y + WINDOW_HEIGHT) / (float)PIXELS_PER_METER ;
    
    gui->current_body = drawing_update(gui->space, gui->current_body, x, y);
  }	
  
  return TRUE;
}

void *client_recv1(void *gui){
  while(1){
    char buff[MAXLINE];
    int bytes = client_recv(((Gui*)gui)->sockfd, buff);
    buff[bytes] = '\0';

    if (buff[0] != '\0'){
      gchar *new_tab = "> ";
      add_to_text(gui, new_tab);
      
      add_to_text(gui, buff);
      
      gchar *new_line = "\n";
      add_to_text(gui, new_line);
    }    

    buff[0] = '\0';

    sleep(.1);
  }
}
