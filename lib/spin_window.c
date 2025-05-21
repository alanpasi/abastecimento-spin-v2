#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include "spin_window.h"
#include "stdpasi.h"

static void on_entry_str_changed(GtkEntry *entry, gpointer user_data) {
    GtkEntryBuffer *buffer = NULL;
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    strcpy(user_data, gtk_entry_buffer_get_text(buffer));
    g_print("Entry changed to %s\n", (char *)user_data);
}

static void on_entry_int_changed(GtkEntry *entry, gpointer user_data) {
    GtkEntryBuffer *buffer = NULL;
    gint *value_ptr = (gint *)user_data;
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    *value_ptr = atoi(gtk_entry_buffer_get_text(buffer));
    g_print("Entry changed to %d\n", *value_ptr);
}

static void on_entry_double_changed(GtkEntry *entry, gpointer user_data) {
    gchar *endptr; // Usado por strtod para verificar erros
    gdouble *value_ptr = (gdouble *)user_data;
    GtkEntryBuffer *buffer = NULL;
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry));
    *value_ptr = strtod(gtk_entry_buffer_get_text(buffer), &endptr);
    g_print("Entry changed to %.2f\n", *value_ptr);
}

static void save_invoice(GtkButton *button, gpointer user_data) {
    Invoice *invoice = (Invoice *)user_data;
    g_print("%d\n", invoice->record_count);
    g_print("%s\n", (char *)invoice->date[invoice->record_count]);
    g_print("%d\n", invoice->odometer[invoice->record_count]);
    g_print("%.2f\n", invoice->unit_price[invoice->record_count]);
    g_print("%.1f\n", invoice->liters[invoice->record_count]);
    g_print("%.2f\n", invoice->total_amount[invoice->record_count]);
}

static void activate(GApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *entry_date;
    GtkWidget *entry_odometer;
    GtkWidget *entry_price;
    GtkWidget *entry_liters;
    GtkWidget *entry_amount;
    GtkWidget *label_record_count;

    GtkEntryBuffer *buffer = NULL;

    char str_value[32] = {0};

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

    // Cria as GtkEntry
    entry_date = GTK_WIDGET(gtk_builder_get_object(builder, "entry_date"));
    g_signal_connect(entry_date, "changed", G_CALLBACK(on_entry_str_changed), &invoice->date[invoice->record_count]);
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_date));
    gtk_entry_buffer_set_text(buffer, (char *)invoice->date[invoice->record_count - 1], -1);

    entry_odometer = GTK_WIDGET(gtk_builder_get_object(builder, "entry_odometer"));
    g_signal_connect(entry_odometer, "changed", G_CALLBACK(on_entry_int_changed), &invoice->odometer[invoice->record_count]);
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_odometer));
    snprintf(str_value, sizeof(str_value), "%d", invoice->odometer[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    entry_price = GTK_WIDGET(gtk_builder_get_object(builder, "entry_price"));
    g_signal_connect(entry_price, "changed", G_CALLBACK(on_entry_double_changed), &invoice->unit_price[invoice->record_count]);
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_price));
    snprintf(str_value, sizeof(str_value), "%.2f", invoice->unit_price[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    entry_liters = GTK_WIDGET(gtk_builder_get_object(builder, "entry_liters"));
    g_signal_connect(entry_liters, "changed", G_CALLBACK(on_entry_double_changed), &invoice->liters[invoice->record_count]);
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_liters));
    snprintf(str_value, sizeof(str_value), "%.1f", invoice->liters[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    entry_amount = GTK_WIDGET(gtk_builder_get_object(builder, "entry_amount"));
    g_signal_connect(entry_amount, "changed", G_CALLBACK(on_entry_double_changed), &invoice->total_amount[invoice->record_count]);
    buffer = gtk_entry_get_buffer(GTK_ENTRY(entry_amount));
    snprintf(str_value, sizeof(str_value), "%.2f", invoice->total_amount[invoice->record_count - 1]);
    gtk_entry_buffer_set_text(buffer, str_value, -1);

    // Cria GtkButton
    button = GTK_WIDGET(gtk_builder_get_object(builder, "button_save"));
    g_signal_connect(button, "clicked", G_CALLBACK(save_invoice), invoice);

    // Cria GtkLabel
    label_record_count = GTK_WIDGET(gtk_builder_get_object(builder, "label_record_count"));
    snprintf(str_value, sizeof(str_value), "[%d] records", invoice->record_count);
    gtk_label_set_text(GTK_LABEL(label_record_count), str_value);

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
