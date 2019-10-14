//
// Created by matej on 10/14/19.
//

#ifndef ISA_SIM_CPP_ISA_SIMULATOR_H
#define ISA_SIM_CPP_ISA_SIMULATOR_H

#include <vector>
#include <map>
#include "instruction_decoder.h"

class ISA_Simulator {
public:
    ISA_Simulator ();
    bool loadFile (const char * filepath);
    bool executeInstruction ();
private:
    unsigned int pc;
    std::vector<unsigned int> raw_insts;
    std::map<unsigned int, InstructionDecoder*> opcode_map;
};


#endif //ISA_SIM_CPP_ISA_SIMULATOR_H
