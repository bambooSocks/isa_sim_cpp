#include <iostream>

#include "isa_simulator.h"


int main () {
    ISA_Simulator sim;
    if (sim.loadFile("../test/file.o")) {
        while (sim.executeInstruction());
    }
    return 0;
}
