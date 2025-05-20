#include <gtk/gtk.h>
#include <stdio.h>

#include "init_gtk.h"
#include "stdpasi.h"

static void click_cb(GtkButton *button, gpointer user_data) {
    g_print("Clicked -> %s\n", (char *)user_data);
}

static void click_entry(GtkEntry *entry, gpointer user_data) {
    GtkEntryBuffer *buffer;
    const gchar *text;
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    text = gtk_entry_buffer_get_text (buffer);
    g_print("Clicked -> %s\n", text);
}

static void activate(GApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *separator;
    GtkWidget *button;
    GtkWidget *entry_date;
    GtkWidget *entry_odometer;
    GtkWidget *entry_price;
    GtkWidget *entry_liters;
    GtkWidget *entry_amount;
    // GtkWidget *button2;
    // GtkWidget *button3;

    GtkEntryBuffer *buffer;

    char str_value[32];

    Invoice *invoice = (Invoice *)user_data;

    GtkBuilder *builder;
    // GtkCssProvider *provider;

    // GdkDisplay *display;

    // Construct a GtkBuilder instance and load our UI description
    builder = gtk_builder_new_from_file("spin_window.ui");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(app));
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);
    gtk_window_set_title(GTK_WINDOW(window), "Abastecimento Spin v2");

    // Cria um GtkLabel
    label = GTK_WIDGET(gtk_builder_get_object(builder, "label_date"));
    // gtk_label_set_text(GTK_LABEL(label), "Label 1...");

    // Cria uma GtkEntry
    entry_date = GTK_WIDGET(gtk_builder_get_object(builder, "entry_date"));
    g_signal_connect(entry_date, "activate", G_CALLBACK(click_entry), "Enter!!!");
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_date));
    gtk_entry_buffer_set_text(buffer, (char *)invoice->date[invoice->record_count - 1], -1);

    entry_odometer = GTK_WIDGET(gtk_builder_get_object(builder, "entry_odometer"));
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_odometer));
    sprintf(str_value, "%d", invoice->odometer[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    entry_price = GTK_WIDGET(gtk_builder_get_object(builder, "entry_price"));
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_price));
    sprintf(str_value, "%.2f", invoice->unit_price[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    entry_liters = GTK_WIDGET(gtk_builder_get_object(builder, "entry_liters"));
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_liters));
    sprintf(str_value, "%.1f", invoice->liters[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    entry_amount = GTK_WIDGET(gtk_builder_get_object(builder, "entry_amount"));
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_amount));
    sprintf(str_value, "%.2f", invoice->total_amount[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    // Cria um GtkSeparator
    separator = GTK_WIDGET(gtk_builder_get_object(builder, "separator"));
    gtk_widget_add_css_class(GTK_WIDGET(separator), "separator");

    // Cria GtkButton
    button = GTK_WIDGET(gtk_builder_get_object(builder, "button_save"));
    g_signal_connect(button, "clicked", G_CALLBACK(click_cb), "Button Save Invoice!!!");

    gtk_window_present(GTK_WINDOW (window));
}

int init_gtk(int argc, char *argv[], Invoice *invoice) {
    GtkApplication *app = NULL;
    gint status = 0;

    // Create a GtkApplication instance
    app = gtk_application_new("com.alanpasi.abastecimento_spin",
                                G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), invoice);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}
