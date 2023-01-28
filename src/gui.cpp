#include "gui.h"

extern const string ProgramName = "KbdMod";
extern GtkTextBuffer *text_buffer;
gboolean quit = false;

void activate(GtkApplication *app, gpointer user_data) {
    init_window(app, user_data);
    string device_name = getDeviceName();
    string reply;
    int retcode = system(("shell/pullKeyboard.sh " + device_name).c_str());
    switch (retcode) {
        case 0:
            loadKeyboard(device_name);
            break;
        case 1:
            reply = Ginput("Warning: no internet connection. Do you want to retry? (y/n) otherwise the keyboard creator will start.");
            if (reply == "y") {

            }
            createKeyboard(device_name);
            break;
        case 2:
            Gprintln("Keyboard not found. Creating new keyboard.");
            createKeyboard(device_name);
            break;
        default:
            throw std::runtime_error("Error: unknown error code when pulling keyboard.");
    }
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

    //remove the temporary files
    system("rm /tmp/devices.txt");
    system("rm /tmp/keyboard.txt");
    Gclear();
    Gprintln("Selected device: " + keyboard_name);
    return keyboard_name;
}