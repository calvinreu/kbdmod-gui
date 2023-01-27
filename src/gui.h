#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>

using std::vector;
using std::string;

struct Key {
    float width;
    float height;
    int keyCode;
    GtkWidget* button;
};

struct Keyboard {
    string name;
    vector<vector<Key>> keys;
    //scaling factor
    float scale;
};

extern Keyboard keyboard;

//load the keyboard from a file to the keyboard vector
extern void loadKeyboard(string filename);

//create the keyboard with a gui
extern void createKeyboard(string name);

//init window
extern void init_window();
