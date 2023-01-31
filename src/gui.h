#pragma once
#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "io.h"
#include "keyboard.h"

using std::vector;
using std::string;
using std::to_string;

//activation func
extern void activate(GtkApplication *app, gpointer user_data);
extern void quit_callback(GtkWidget *widget, gpointer data);

//init window
extern void init_window(GtkApplication *app, gpointer user_data);

//output device names
extern string getDeviceName();

