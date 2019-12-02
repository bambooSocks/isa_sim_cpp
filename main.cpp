// main.cpp -

#include <iostream>
#include "isa_simulator.h"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "\x1B[1;31mNo input binary file\x1B[0m\r\n";
        std::cerr << "\x1B[1;31mTerminated with exit code: 3\x1B[0m\r\n\r\n";
        exit(3);
    }
    ISA_Simulator sim;
    if (sim.loadFile(argv[1])) {
        while (sim.executeInstruction() == EXEC_OK);
    }
    return 0;
}
