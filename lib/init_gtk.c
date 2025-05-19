#include <gtk/gtk.h>
#include "init_gtk.h"

static void activate(GApplication *app) {
    GtkWidget *window;
    // GtkWidget *label1;
    // GtkWidget *separator1;
    // GtkWidget *button1;
    // GtkWidget *button2;
    // GtkWidget *button3;
    // GtkWidget *entry1;

    GtkBuilder *builder;
    // GtkCssProvider *provider;

    // GdkDisplay *display;

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new_from_file("spin_window.ui");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Abastecimento Spin v2");

    gtk_window_present(GTK_WINDOW (window));
}

int init_gtk(int argc, char *argv[]) {
    GtkApplication *app = NULL;
    gint status = 0;

    // Create a GtkApplication instance
    app = gtk_application_new("com.alanpasi.abastecimento_spin",
                                G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    /* Run the application. */
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
