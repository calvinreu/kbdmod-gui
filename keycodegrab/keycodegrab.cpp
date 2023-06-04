#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    input_event ev;
    if (argc != 3) {
        std::cout << 
'Usage: keycodegrab <output file name>\nto quit press 1 and 0 at the same time'
        << std::endl;
        return 1;
    }
    std::string outputFileName = argv[2];

    std::ofstream outputFile;
    outputFile.open(outputFileName);
    if(!outputFile.is_open()) {
        std::cout << "Error opening output file" << std::endl;
        return 1;
    }
    outputFile.clear();
    u_int16_t prevcode=0;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    while(true) {
        if(fread(&ev, sizeof(input_event), 1, stdin) != 1) {
            std::cout << "Error reading input event" << std::endl;
            return -1;
        }
        fwrite(&ev, sizeof(input_event), 1, stdout);
        if(ev.type == EV_KEY && ev.value == 1) {
            outputFile << ev.code << std::endl;
            //quit if 1 and 0 are pressed at the same time
            if(ev.code == KEY_1 && prevcode == KEY_0 ||
               ev.code == KEY_0 && prevcode == KEY_1) {
                break;
            }
            prevcode = ev.code;
        }
    }

    //close output file
    outputFile.close();

    return 0;        
}