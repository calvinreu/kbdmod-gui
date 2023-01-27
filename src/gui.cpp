#include "gui.h"

extern const string ProgramName = "KbdMod";
extern GtkTextBuffer *text_buffer;
gboolean quit = false;

void activate(GtkApplication *app, gpointer user_data) {
    init_window(app, user_data);
    getDeviceName();

}

void quit_callback(GtkWidget *widget, gpointer data) {
    quit = true;
    IO_quit_callback(widget, data);
    //send event to finish the loop
    g_application_quit(G_APPLICATION(widget));
}

string getDeviceName() {
    //inform about the reason for admin rights
    gtk_text_buffer_insert_at_cursor(text_buffer, "Superuser rights are required to access the libinput device names.\n", -1);

    //load the input device names
    system("pkexec libinput list-devices | grep Device > /tmp/devices.txt");
    //save keyboard names
    system("cat /tmp/devices.txt | grep eyboard > /tmp/keyboard.txt");

    //select keyboard
    string keyboard_name;

    try {
        keyboard_name = textSelector("Select the keyboard you want to use: ", "/tmp/keyboard.txt", "Not listed See all devices");
    }catch (std::runtime_error& e) {
        throw e;
    }
    if (keyboard_name == "Not listed") {
        try {
            keyboard_name = textSelector("Select the keyboard you want to use: ", "/tmp/devices.txt", "Not listed");
        }catch (std::runtime_error& e) {
            throw e;
        }
        if (keyboard_name == "Exit if not listed") {
            throw std::runtime_error("Error: Keyboard not found.");
        }
    }

    return keyboard_name;
    //check if keyboard is available
    //if (system("shell/pullKeyboard.sh") == 0) {
    //    loadKeyboard(keyboard_name);
    //}else{
    //    createKeyboard(keyboard_name);
    //}
}