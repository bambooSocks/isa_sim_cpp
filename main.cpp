#include <iostream>

#include "isa_simulator.h"


int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No input binary file\r\n";
    }
    ISA_Simulator sim;
    if (sim.loadFile(argv[1])) {
        while (sim.executeInstruction() == EXEC_OK);
    }
    return 0;
}
