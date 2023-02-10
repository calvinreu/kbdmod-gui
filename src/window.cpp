#include "gui.h"

#ifdef DEBUG
#include <iostream>
#endif

GtkWidget *keyboard_space;
//text buffer
GtkTextBuffer *text_buffer;
//text field
GtkWidget *text_field;

const string ProgramName = "KbdMod";

void init_window(GtkApplication *app, gpointer user_data) {

    //create window
    GtkWindow* window = GTK_WINDOW(gtk_application_window_new(app));
    if(window==NULL)
    {
        throw std::runtime_error("Error: Failed to create the window.");
    }
    gtk_window_set_title(window, ProgramName.c_str());
    gtk_window_set_default_size(window, 800, 600);

    //conect signals
    g_signal_connect(window, "destroy", G_CALLBACK(quit_callback), NULL);

    //create main box
    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    if(main_box==NULL)
    {
        throw std::runtime_error("Error: Failed to create the main box.");
    }

    // Text field
    text_field = gtk_text_view_new();
    if(text_field==NULL)
    {
        throw std::runtime_error("Error: Failed to create the text field.");
    }
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_field), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_field), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_field), GTK_WRAP_WORD);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_field), 10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_field), 10);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_field), 10);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_field), 10);
    gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(text_field), FALSE);

    
    //make it scrollable
    GtkScrolledWindow* scroll = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new());
    //scrollbox size request
    gtk_widget_set_size_request(GTK_WIDGET(scroll), -1, 300);
    gtk_scrolled_window_set_child(scroll, GTK_WIDGET(GTK_TEXT_VIEW(text_field)));

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_field));
    
    keyboard_space = gtk_fixed_new();
    if(keyboard_space==NULL)
    {
        throw std::runtime_error("Error: Failed to create the keyboard space.");
    }
    gtk_widget_set_size_request(keyboard_space, -1, 400);

    //append to main box
    gtk_box_append(GTK_BOX(main_box), GTK_WIDGET(scroll));
    gtk_box_append(GTK_BOX(main_box), keyboard_space);

    //append main box to window
    gtk_window_set_child(window, main_box);

    //show window
    gtk_widget_show(GTK_WIDGET(window));
}
