#include "io.h"

gboolean input_ready;
string input;

string textSelector(string text, string options_file, string additional_option = "") {
    //load options from file
    std::ifstream options(options_file);
    //error if file not found
    if (!options) {
        throw std::runtime_error("Error: File "+ options_file +" not found.");
    }
    //parse to vector and use other function
    vector<string> options_vector;
    for (string line; std::getline(options, line);)
        options_vector.push_back(line);
    //add additional option
    if (additional_option != "") {
        //add option per line
        std::istringstream iss(additional_option);
        for (string line; std::getline(iss, line);)
            options_vector.push_back(line);
    }
    try {
        return textSelector(text, options_vector);
    }catch (std::runtime_error& e) {
        throw e;
    }
}

string textSelector(string text, vector<string> options) {
    //add text to text field
    gtk_text_buffer_insert_at_cursor(text_buffer, text.c_str(), -1);
    //create selection options in the text field
    for (int i = 0; i < options.size(); i++) {
        gtk_text_buffer_insert_at_cursor(text_buffer, string("(" + std::to_string(i) + ") " + options[i]).c_str(), -1);
    }
    //get user from text field
    gtk_text_buffer_insert_at_cursor(text_buffer, "Enter the number of the option you want to use: ", -1);    
    //parse to int
    int option_number = std::stoi(gtk_text_buffer_get_text(text_buffer, 0, 0, FALSE));
    //check if number is valid
    if (option_number >= options.size()) {
        throw std::runtime_error("Error: Invalid number.");
    }
    return options[option_number];
}

string Ginput(string text = "") {
    gtk_text_buffer_insert_at_cursor(text_buffer, text.c_str(), -1);
    input_ready = false;
    while (!input_ready) {
        gtk_main_iteration();
    }
    return input;
}

void Ginput_callback(GtkWidget *widget, gpointer data) {
    input = gtk_entry_get_text(GTK_ENTRY(widget));
    input_ready = true;
}