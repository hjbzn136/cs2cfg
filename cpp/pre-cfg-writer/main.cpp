#include <windows.h>
#include <iostream>
#include "command_processor.h"

int main() {
    SetConsoleOutputCP(65001); 
    CommandProcessor processor("autoexec.txt", "game/csgo/cfg/autoexec.cfg");
    processor.process();
    return 0;
}
