// main.cpp
// Matej Majtan (s184457) & Søren Tønnesen (s180381)
// 02-12-2019

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
