#include "io.h"

gboolean input_ready;
extern gboolean quit;
string input;

extern GtkTextBuffer *text_buffer;
extern GtkWidget *text_field;

string textSelector(string text, string options_file, string additional_option) {
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
    Gprintln(text);
    for (int i = 0; i < options.size(); i++) {
        Gprintln("(" + std::to_string(i) + ") " + options[i]);
    }
    //parse to int
    int option_number;
    string input_text;
    try{
        input_text = Ginput("Enter the number of the option you want to use: ");
        option_number = std::stoi(input_text);
    }catch (std::invalid_argument& e) {
        Gprintln("\nError: " + string(e.what()) + "\n input: " + input_text);
        //check if program is running
        if (quit)
            return "";
        return textSelector(text, options);
    }
    //check if number is valid
    if (option_number >= options.size()) {
        throw std::runtime_error("Error: Invalid number.");
    }
    return options[option_number];
}

void Ginput_callback(GtkWidget *widget, gpointer data) {
    GtkTextView* text_view = (GtkTextView*) data;
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);
    gchar* text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);
    //check if last charachter is a new line
    if (text[strlen(text) - 1] == '\n') {
        g_free(text);
        return;
    }
    gchar** lines = g_strsplit(text, "\n", -1);
    input = g_strdup(lines[g_strv_length(lines) - 1]);
    g_free(text);
    g_strfreev(lines);
    input_ready = TRUE;
}

string Ginput(string text) {
    //if text is multiline print error
    if (text.find("\n") != std::string::npos) {
        Gprintln("Error: Ginput does not support multiline text.");
    }else{
        Gprint(text);
    }

    input_ready = false;
    g_signal_connect(text_buffer, "changed", G_CALLBACK(Ginput_callback), text_field);
    while (!input_ready) {
        g_main_context_iteration(NULL, TRUE);
    }
    //remove text from input
    input.erase(0, text.length());
    return input;
}

void Gclear() {
    gtk_text_buffer_set_text(text_buffer, "", -1);
}

//print to gui
void Gprint(string text) {
    gtk_text_buffer_insert_at_cursor(text_buffer, text.c_str(), -1);
}
void Gprintln(string text) {
    gtk_text_buffer_insert_at_cursor(text_buffer, (text + " \n").c_str(), -1);
}

void IO_quit_callback(GtkWidget *widget, gpointer data) {
    input_ready = TRUE;
}