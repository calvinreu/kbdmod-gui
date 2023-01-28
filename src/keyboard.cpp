#include "gui.h"

extern Keyboard keyboard;
extern GtkWidget *keyboard_space;

const uint marginW = 3;
const uint marginH = 3;

string device_to_file(string device) {
    return "~/.kbdmod/boards/" + device + ".json";
}

void loadKeyboard(string name) {
    string filename = device_to_file(name);
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file " + filename);
    }
    //parse json
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(file, root)) {
        throw std::runtime_error("Error: could not parse file " + filename);
    }

    float totalW = 0;
    float totalH = 0;

    //load rows
    Json::Value rows = root["rows"];
    for (int i = 0; i < rows.size(); i++) {
        Row row;
        row.height = rows[i]["height"].asFloat();
        totalH += row.height;
        float tmpW = 0;
        //load keys
        Json::Value keys = rows[i]["keys"];
        for (int j = 0; j < keys.size(); j++) {
            Key key;
            key.width = keys[j]["w"].asFloat();
            key.keyCode = keys[j]["kc"].asInt();
            row.keys.push_back(key);
            tmpW += key.width;
        }
        keyboard.rows.push_back(row);
        if (tmpW > totalW)
            totalW = tmpW;
    }

    //calculate scaling factor
    //get keyboard size from gui
    GtkAllocation keyboardSize;
    gtk_widget_get_allocation(keyboard_space, &keyboardSize);
    //calculate scaling factor
    keyboardSize.width = (keyboardSize.width-marginW) / totalW;
    keyboardSize.height = (keyboardSize.height-marginH) / totalH;
    if (keyboardSize.width < keyboardSize.height)
        keyboard.scale = keyboardSize.width;
    else
        keyboard.scale = keyboardSize.height;

    //create buttons
    double y = marginH;
    int w, h = 0;
    for (auto i = keyboard.rows.begin(); i != keyboard.rows.end(); i++) {
        double x = marginW;
        for (auto j = i->keys.begin(); j != i->keys.end(); j++) {
            j->button = gtk_button_new_with_label(to_string(j->keyCode).c_str());
            w = (j->width*keyboard.scale) - marginW;
            h = (i->height*keyboard.scale) - marginH;

            gtk_widget_set_size_request(j->button, w, h);
            gtk_fixed_put(GTK_FIXED(keyboard_space), j->button, x, y);
        }
        y += i->height*keyboard.scale;
    }
}

void createKeyboard(string name) {

}