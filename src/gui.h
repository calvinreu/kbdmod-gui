#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "io.h"

using std::vector;
using std::string;
using std::to_string;

extern Keyboard keyboard;

//activation func
extern void activate(GtkApplication *app, gpointer user_data);
extern void quit_callback(GtkWidget *widget, gpointer data);

//load the keyboard from a file to the keyboard vector
extern void loadKeyboard(string filename);

//create the keyboard with a gui
extern void createKeyboard(string name);

//init window
extern void init_window(GtkApplication *app, gpointer user_data);

//output device names
extern string getDeviceName();

