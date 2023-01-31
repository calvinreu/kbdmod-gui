#include <string>
#include <cstdlib>
#include "gui.h"

using std::string;

#ifndef VERSION
#define VERSION "build error version unknown"
#endif

KeyboardBaseboard keyboard;
extern GtkWidget *text_field;
extern GtkWidget *keyboard_space;
extern GtkTextBuffer *text_buffer;


int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    //create application
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    if(app==NULL)
    {
        throw std::runtime_error("Error: Failed to create the application.");
    }

    //connect signals
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    //run application
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);

    return status;
}