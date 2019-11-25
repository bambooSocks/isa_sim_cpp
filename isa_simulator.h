//
// Created by matej on 10/14/19.
//

#ifndef ISA_SIM_CPP_ISA_SIMULATOR_H
#define ISA_SIM_CPP_ISA_SIMULATOR_H

#include <vector>
#include <map>
#include "instruction_decoder.h"
#include "register_file.h"

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
    void terminate (std::string msg);
    void terminateWithError (std::string msg, int exit_code);
    unsigned int pc;
    RegisterFile *registerFile;
    std::vector<unsigned int> raw_insts;
    std::map<unsigned int, InstructionDecoder*> opcode_map;
};


#endif //ISA_SIM_CPP_ISA_SIMULATOR_H
