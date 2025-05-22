#include <gtk/gtk.h>
#include "stdpasi.h"

int initWindow(int, char *[], Invoice *);
static void activate(GApplication *, gpointer);
static void click_entry(GtkEntry *, gpointer);
static void click_cb(GtkButton *, gpointer);
