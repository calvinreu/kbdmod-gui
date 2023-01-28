#include <map>
#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "io.h"

using std::map;
using std::string;
using std::to_string;
using std::vector;

struct Key
{
    float width;
    int keyCode;
};

struct Row
{ // change row width
    void changeRowWidth(int row, float width);
    float height;
    float width;
    vector<Key> keys;
};

class KeyboardBaseboard
{
public:
    struct index
    {
        // set to -1 if not used
        int row;
        int key;
    };
private:
    string name;
    vector<Row> rows;
    // scaling factor
    float scale, width, height;

    // gtk button map
    map<GtkWidget *, KeyboardBaseboard::index> buttonMap;

    KeyboardBaseboard::index updownkey;

    // callback func
    void(*callback)(GtkWidget *, gpointer) = nullptr;

    // draw keyboard
    void drawKeyboard();
    // clear keyboard
    void clearKeyboard();
    // redraw keyboard
    void redrawKeyboard();
    // draw row
    void drawRowFrom(int row, int from = 0);
    // clear row
    void clearRowFrom(int row, int from = 0);
    // redraw row
    void redrawRowFrom(int row, int from = 0);

    // calculate scaling factor return true if changed
    bool calculateScale();

public:
    //constructor init with 0
    KeyboardBaseboard();
    //destructor
    ~KeyboardBaseboard();

    void loadKeyboard(string name);
    void createKeyboard(string name);

        // set selected key keycode
    void setKeyCode(int keyCode);
    // set selected key width(not allowed on updownkey if present)
    float setKeyWidth(float width);
    // set selected key name only gui change has to be saved in mappings
    void setKeyName(string name);
    // set key color
    void setKeyColor(string color);

    // save keyboard to file
    void saveKeyboard(string name);
};

// select key when clicked as callback function
extern void key_kbcreator_callback(GtkWidget *widget, gpointer data);
//has to be moved to mappings
extern void key_mapping_callback(GtkWidget *widget, gpointer data);
