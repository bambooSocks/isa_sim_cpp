#include <iostream>

#include "isa_simulator.h"


int main () {
    ISA_Simulator sim;
    if (sim.loadFile("../test/loop.bin")) {
        while (sim.executeInstruction() == EXEC_OK);
    }
    return 0;
}
