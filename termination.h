//
// Created by matej on 11/29/19.
//

#ifndef ISA_SIM_CPP_TERMINATION_H
#define ISA_SIM_CPP_TERMINATION_H


#include "register_file.h"

class Termination {
private:
    RegisterFile *registerFile;
public:
    Termination ();
    void terminate (const std::string& msg, int exit_code);
};


#endif //ISA_SIM_CPP_TERMINATION_H
