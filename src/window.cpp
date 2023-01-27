#include "gui.h"

GtkWidget *window;
GtkWidget *text_field;
GtkEventController *keyboard_space;
//text buffer
GtkTextBuffer *text_buffer;
GtkWidget *box;
GtkWidget *scroll;

const string ProgramName = "KbdMod";

void init_window(GtkApplication *app, gpointer user_data) {

    //create window
    window = gtk_application_window_new(app);
    if(window==NULL)
    {
        throw std::runtime_error("Error: Failed to create the window.");
    }
    gtk_window_set_title(GTK_WINDOW(window), ProgramName.c_str());
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    //conect signals
    g_signal_connect(window, "destroy", G_CALLBACK(quit_callback), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    if(box==NULL)
    {
        throw std::runtime_error("Error: Failed to create the box container.");
    }
    gtk_window_set_child(GTK_WINDOW(window), box);

    // controller for keyboard_space
    keyboard_space = gtk_event_controller_key_new();
    gtk_widget_add_controller(box, keyboard_space);
    if(keyboard_space==NULL)
    {
        throw std::runtime_error("Error: Failed to create the keyboard space.");
    }
    gtk_widget_set_size_request(box, -1, 300);

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
    scroll = gtk_scrolled_window_new();
    //scrollbox size request
    gtk_widget_set_size_request(scroll, -1, 300);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), text_field);

    //add to box
    gtk_box_append(GTK_BOX(box), scroll);

    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_field));

    //show window
    gtk_widget_show(window);
}
