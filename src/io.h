#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

using std::vector;
using std::string;

extern GtkTextBuffer *text_buffer;

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
inline void Gprint(string text) {
    gtk_text_buffer_insert_at_cursor(text_buffer, text.c_str(), -1);
}
inline void Gprintln(string text) {
    gtk_text_buffer_insert_at_cursor(text_buffer, (text + " \n").c_str(), -1);
}

//input from gui
extern string Ginput(string text = "");