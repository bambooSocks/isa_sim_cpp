// isa_simulator.h
// Matej Majtan (s184457) & Søren Tønnesen (s180381)
// 02-12-2019

#ifndef ISA_SIM_CPP_ISA_SIMULATOR_H
#define ISA_SIM_CPP_ISA_SIMULATOR_H

#include <vector>
#include <map>
#include "instruction_decoder.h"
#include "register_file.h"
#include "isa_simulator.h"
#include "termination.h"

typedef enum {
    EXEC_OK,
    EXEC_ERROR,
    EXEC_EOF,
    EXEC_ECALL
} exec_result_t;


class ISA_Simulator {
public:
    ISA_Simulator ();
    bool loadFile (const char * filepath);
    exec_result_t executeInstruction ();
private:
    unsigned int pc;
    Termination *term;
    RegisterFile *registerFile;
    std::vector<unsigned int> inst_mem;
    std::map<unsigned int, InstructionDecoder*> opcode_map;
};


#endif //ISA_SIM_CPP_ISA_SIMULATOR_H
