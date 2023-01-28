#include <map>
#include <vector>
#include <string>
#include <gtk/gtk.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <json/json.h>

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
private:
    // exeption for one keyslot up down in a row wrap two keys onto the same slot
    struct index
    {
        // set to -1 if not used
        int row;
        int key;
    };

    string name;
    vector<Row> rows;
    // scaling factor
    float scale, width, height;

    // gtk button map
    map<GtkWidget *, KeyboardBaseboard::index> buttonMap;

    // currently selected
    Key *selectedKey;
    GtkWidget *selectedButton;
    Row *selectedRow;

    KeyboardBaseboard::index updownkey;

    // draw keyboard
    void drawKeyboard();
    // clear keyboard
    void clearKeyboard();
    // draw row
    void drawRow(int row);
    // clear row
    void clearRow(int row);

    // calculate scaling factor return true if changed
    bool calculateScale();

public:
    void loadKeyboard(string name);
    void createKeyboard(string name);

    // select key when clicked as callback function
    void selectKey(GtkWidget *widget, gpointer data);

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
