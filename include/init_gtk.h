#include <gtk/gtk.h>
#include "stdpasi.h"

int init_gtk(int, char *[], Invoice *);
static void activate(GApplication *, gpointer);
static void click_entry(GtkEntry *, gpointer);
static void click_cb(GtkButton *, gpointer);
