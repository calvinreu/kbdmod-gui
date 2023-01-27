#include "gui.h"

GtkWidget *window;
GtkWidget *text_field;
GtkEventController *keyboard_space;

const string ProgramName = "KbdMod";

void init_window() {

    //create window
    window = gtk_window_new();
    if(window==NULL)
    {
        throw std::runtime_error("Error: Failed to create the window.");
    }
    gtk_window_set_title(GTK_WINDOW(window), ProgramName.c_str());
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
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
    gtk_box_append(GTK_BOX(box), text_field);

    //set background color
    GtkStyleContext *style;
    GdkRGBA color;

    style = gtk_widget_get_style_context(window);
    //get color from gtk theme
    gtk_style_context_lookup_color(style, "theme_bg_color", &color);
    //set color
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);


    //show window
    gtk_widget_show(box);
    gtk_widget_show(text_field);
    gtk_widget_show(window);
}
