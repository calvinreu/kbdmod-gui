#include "keyboard.h"

extern KeyboardBaseboard keyboard;
extern GtkWidget *keyboard_space;
extern gboolean quit;

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
    for (auto &button : buttonMap) {
        gtk_widget_unparent(button.first);
    }
    //clear button map
    buttonMap.clear();
}

void KeyboardBaseboard::drawKeyboard() {
    float y = marginH;
    int width, height;
    //draw rows
    for (int i = 0; i < keyboard.rows.size(); i++) {
        //draw row
        float x = marginW;
        height = (keyboard.rows[i].height * keyboard.scale)-marginH;
        for(int j = 0; j < keyboard.rows[i].keys.size(); j++) {
            //create button
            GtkWidget *button = gtk_button_new();
            //calc size
            width = (keyboard.rows[i].keys[j].width * keyboard.scale)-marginW;
            //set size
            gtk_widget_set_size_request(button, width, height);
            //set position
            gtk_fixed_put(GTK_FIXED(keyboard_space), button, x, y);
            //set label
            gtk_button_set_label(GTK_BUTTON(button), to_string(keyboard.rows[i].keys[j].keyCode).c_str());
            //set button map
            buttonMap[button] = {i, j};
            //set callback
            g_signal_connect(button, "clicked", G_CALLBACK(keyboard_callback), NULL);
            //show button
            gtk_widget_show(button);
            //update x position
            x += keyboard.rows[i].keys[j].width * keyboard.scale;
        }
        //update y position
        y += keyboard.rows[i].height * keyboard.scale;
    }  
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
    name = name_;
    //ask for line count
    int lineCount = 0;
    lineCount = Ginput<int>("How many rows of keys does your keyboard have?");

    //ask for line height and key count for each line
    for (int i = 0; i < lineCount; i++) {
        Gclear();
        Gprintln("Current row: " + to_string(i+1));

        Row row;
        //ask for line height
        row.height = Ginput<float>("What is the height of row " + to_string(i+1) + "?");
        Gprintln("Enter width then amount of those keys from left to right. After the line is done enter 0 for width.");
        keyboard.height += row.height;
        row.width = 0;
        float width = 1;
        while (width != 0.0 && !quit) {
            //ask for widthxkey count
            width = Ginput<float>("Width of next key(s)?");
            int keyCount = Ginput<int>("How many keys with this width?");

            for (int j = 0; j < keyCount; j++) {
                Key key;
                key.width = width;
                key.keyCode = 0;
                row.keys.push_back(key);
            }

            #ifdef DEBUG
            Gprintln("Width: " + to_string(width));
            Gprintln("Key count: " + to_string(keyCount));
            #endif
        }

        keyboard.rows.push_back(row);
        if (row.width > keyboard.width)
            keyboard.width = row.width;
    }

    //init keycodes
    system("shell/getKeycodes.bin > /tmp/keycodes.txt");
    //load keycodes
    std::ifstream file("/tmp/keycodes.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open file /tmp/keycodes.txt");
    }
    //loop through keycodes and set them to the keys left to right top to bottom
    int keycode = 0;
    for (int i = 0; i < keyboard.rows.size(); i++) {
        for (int j = 0; j < keyboard.rows[i].keys.size(); j++) {
            file >> keycode;
            keyboard.rows[i].keys[j].keyCode = keycode;
        }
    }
    
    calculateScale();
    drawKeyboard();
    //save keyboard
    saveKeyboard();
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

void keyboard_callback(GtkWidget *widget, gpointer data) {

}