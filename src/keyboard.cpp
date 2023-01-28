#include "keyboard.h"

extern KeyboardBaseboard keyboard;
extern GtkWidget *keyboard_space;

const uint marginW = 3;
const uint marginH = 3;

inline string device_to_file(string device) {
    return "~/.kbdmod/boards/" + device + ".json";
}

void KeyboardBaseboard::drawKeyboard() {
    //clear keyboard
    clearKeyboard();
    //draw rows
    for (int i = 0; i < keyboard.rows.size(); i++)
        drawRow(i);
}

void KeyboardBaseboard::clearKeyboard() {
    //remove all buttons
    GList *children, *iter;
    children = (GTK_EVENT_CONTROLLER(keyboard_space));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_container_remove(GTK_CONTAINER(keyboard_space), GTK_WIDGET(iter->data));
    g_list_free(children);
    //clear button map
    buttonMap.clear();
}

bool KeyboardBaseboard::calculateScale() {
    //get keyboard size from gui
    GtkAllocation keyboardSize;
    gtk_widget_get_allocation(keyboard_space, &keyboardSize);

    //calculate scaling factor
    float scalew = (keyboardSize.width-marginW) / keyboard.width;
    float scaleh = (keyboardSize.height-marginH) / keyboard.height;
    float scale_ = 0;
    //set scale to the smaller one to fit the keyboard in the window
    if (scalew < scaleh)
        scale_ = keyboardSize.width;
    else
        scale_ = keyboardSize.height;

    if (scale_ != keyboard.scale) {
        keyboard.scale = scale_;
        return true;
    }
    return false;
}

void KeyboardBaseboard::loadKeyboard(string name) {
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
        row.width = 0;
        //load keys
        Json::Value keys = rows[i]["keys"];
        for (int j = 0; j < keys.size(); j++) {
            Key key;
            key.width = keys[j]["w"].asFloat();
            key.keyCode = keys[j]["kc"].asInt();
            row.keys.push_back(key);
            row.width += key.width;
        }
        keyboard.rows.push_back(row);
        if (row.width > totalW)
            totalW = row.width;
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

void KeyboardBaseboard::createKeyboard(string name) {

}