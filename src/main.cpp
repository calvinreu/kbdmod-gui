#include <string>
#include <cstdlib>
#include "gui.h"

using std::string;

#ifndef VERSION
#define VERSION "build error version unknown"
#endif


int main(int argc, char *argv[])
{
    gtk_init();
    init_window();

    system("sleep 1");

    return 0;
}