//
// Created by matej on 11/29/19.
//

#include <iostream>
#include "termination.h"

void Termination::terminate (const std::string &msg, int exit_code) {
    registerFile->dump_registers();
    if (exit_code == 0) {
        std::cout << "\x1B[1;32m" << msg << "\x1B[0m\r\n\r\n";
        registerFile->print_registers();
        exit(0);
    } else {
        std::cerr << "\x1B[1;31m" << msg << "\x1B[0m\r\n";
        std::cerr << "\x1B[1;31mTerminated with exit code: " << std::dec << int(exit_code) << "\x1B[0m\r\n\r\n";
        registerFile->print_registers();
        exit(exit_code);
    }
}

Termination::Termination () {
    registerFile = RegisterFile::getInstance();
}
