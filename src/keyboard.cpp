#include "keyboard.h"

extern KeyboardBaseboard keyboard;
extern GtkWidget *keyboard_space;

const uint marginW = 3;
const uint marginH = 3;

inline string device_to_file(string device) {
    return "~/.kbdmod/boards/" + device + ".json";
}

KeyboardBaseboard::KeyboardBaseboard() {
    //set default values
    name = "";
    scale = 0;
    width = 0;
    height = 0;
    updownkey.row = -1;
    updownkey.key = -1;
}

KeyboardBaseboard::~KeyboardBaseboard() {
    //clear keyboard
    clearKeyboard();
}

void KeyboardBaseboard::drawKeyboard() {
    //draw rows
    for (int i = 0; i < keyboard.rows.size(); i++) {
        //draw row
        drawRowFrom(i);
    }  
}

void KeyboardBaseboard::clearKeyboard() {
    //remove all buttons
    for (auto &button : buttonMap) {
        gtk_widget_unparent(button.first);
    }
    //clear button map
    buttonMap.clear();
}

void KeyboardBaseboard::redrawKeyboard() {
    //clear keyboard
    clearKeyboard();
    //draw keyboard
    drawKeyboard();
}

void KeyboardBaseboard::drawRowFrom(int row, int from) {
    //get start position
    float y = marginH;
    for (int i = 0; i < row; i++)
        y += keyboard.rows[i].height * keyboard.scale;
    
    float x = marginW;
    for (int i = 0; i < from; i++)
        x += keyboard.rows[row].keys[i].width * keyboard.scale;
    
    //draw keys
    for (int i = from; i < keyboard.rows[row].keys.size(); i++) {
        //create button
        GtkWidget *button = gtk_button_new();
        //calc size
        int width = (keyboard.rows[row].keys[i].width * keyboard.scale)-marginW;
        int height = (keyboard.rows[row].height * keyboard.scale)-marginH;
        //set size
        gtk_widget_set_size_request(button, width, height);
        //set position
        gtk_fixed_put(GTK_FIXED(keyboard_space), button, x, y);
        //set label
        gtk_button_set_label(GTK_BUTTON(button), to_string(keyboard.rows[row].keys[i].keyCode).c_str());
        //set button map
        buttonMap[button] = {row, i};
        //set callback
        g_signal_connect(button, "clicked", G_CALLBACK(keyboard.callback), NULL);
        //show button
        gtk_widget_show(button);
        //update x position
        x += keyboard.rows[row].keys[i].width * keyboard.scale;
    }
}

void KeyboardBaseboard::clearRowFrom(int row, int from) {
    //iterate over button map
    for (auto &button : buttonMap) {
        //check if button is in row
        if (button.second.row == row && button.second.key >= from) {
            //remove button
            gtk_fixed_remove(GTK_FIXED(keyboard_space), button.first);
        }
    }
    //clear button map
    for (auto it = buttonMap.begin(); it != buttonMap.end();) {
        if (it->second.row == row && it->second.key >= from)
            it = buttonMap.erase(it);
        else
            ++it;
    }
}

void KeyboardBaseboard::redrawRowFrom(int row, int from) {
    //clear row
    clearRowFrom(row, from);
    //draw row
    drawRowFrom(row, from);
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
        scale_ = scalew;
    else
        scale_ = scaleh; 

    if (scale_ != keyboard.scale) {
        keyboard.scale = scale_;
        return true;
    }
    return false;
}

void KeyboardBaseboard::loadKeyboard(string name_) {
    name = name_;
    callback = key_mapping_callback;
    string filename = device_to_file(name_);
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

    //load rows
    Json::Value rows = root["rows"];
    for (int i = 0; i < rows.size(); i++) {
        Row row;
        row.height = rows[i]["height"].asFloat();
        keyboard.height += row.height;
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
        if (row.width > keyboard.width)
            keyboard.width = row.width;
    }

    calculateScale();
    drawKeyboard();
}

void KeyboardBaseboard::createKeyboard(string name_) {
    callback = key_kbcreator_callback;
    name = name_;
    //ask for line count
    int lineCount = 0;
    lineCount = Ginput<int>("How many rows of keys does your keyboard have?");

    //ask for line height and key count for each line
    for (int i = 0; i < lineCount; i++) {
        Row row;
        //ask for line height
        row.height = Ginput<int>("What is the height of row " + to_string(i+1) + "?");
        keyboard.height += row.height;
        row.width = 0;
        //ask for key count
        int keyCount = 0;
        keyCount = Ginput<int>("How many keys does row " + to_string(i+1) + " have?");
        //set row width to key count
        row.width = keyCount;
        //init with keys of width 1
        for (int j = 0; j < keyCount; j++) {
            Key key;
            key.width = 1;
            key.keyCode = 0;
            row.keys.push_back(key);
        }
        keyboard.rows.push_back(row);
        if (row.width > keyboard.width)
            keyboard.width = row.width;
    }

    calculateScale();
    drawKeyboard();
    Gprintln("press on the keys to resize them or enter a index row,column");
}

void KeyboardBaseboard::saveKeyboard() {
    //create json
    Json::Value root;
    Json::Value rows;
    for (int i = 0; i < keyboard.rows.size(); i++) {
        Json::Value row;
        row["height"] = keyboard.rows[i].height;
        Json::Value keys;
        for (int j = 0; j < keyboard.rows[i].keys.size(); j++) {
            Json::Value key;
            key["w"] = keyboard.rows[i].keys[j].width;
            key["kc"] = keyboard.rows[i].keys[j].keyCode;
            keys.append(key);
        }
        row["keys"] = keys;
        rows.append(row);
    }
    root["rows"] = rows;

    //write json to file
    string filename = device_to_file(name);
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file " + filename);
    }
    file << root;
    file.close();
}

void key_kbcreator_callback(GtkWidget* widget, gpointer data) {
    Gclear();
    //get index
    auto index = keyboard.buttonMap[widget];

    //ask for width
    float width = Ginput<float>("What is the width of the key?");
    //calc change
    width -= keyboard.rows[index.row].keys[index.key].width;

    //set row width
    keyboard.rows[index.row].width += width;
    //set width
    keyboard.rows[index.row].keys[index.key].width += width;

    //check if row width is bigger than keyboard width if so recalculate scale
    if (keyboard.rows[index.row].width > keyboard.width) {
        keyboard.width = keyboard.rows[index.row].width;
        if(keyboard.calculateScale()) {
            keyboard.redrawKeyboard();
            return;
        }
    }
    keyboard.redrawRowFrom(index.row, index.key);
    Gclear();
}

void key_mapping_callback(GtkWidget* widget, gpointer data) {
    Gclear();
    
}