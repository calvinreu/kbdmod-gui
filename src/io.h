#pragma once
#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

using std::vector;
using std::string;

/*text selector
text: text to display before the options
options_file: file with options
additional_option: additional option to add to the options new line per option
returns the selected option
*/
extern string textSelector(string text, string options_file, string additional_option="");

//text selector with vector
extern string textSelector(string text, vector<string> options);

//print to gui
extern void Gprintln(string text);
extern void Gprint(string text);

//input from gui text is not allowed to be multiline last line has to be empty
extern string Ginput(string text = "");
extern string GinputClear(string text = "");

template <typename T>
extern T Ginput(string text = "");

//clear
extern void Gclear();

extern void IO_quit_callback(GtkWidget *widget, gpointer data);