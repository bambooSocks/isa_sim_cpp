//
// Created by matej on 10/10/19.
//

#include <iostream>
#include "registerfile.h"

RegisterFile::RegisterFile () {
    for (int i = 0; i < m_reg_file.size(); i++){
        m_reg_file[i] = 0;
    }
}

uint RegisterFile::read(RegisterFile::Register reg) {
    if (reg == x0) {
        return 0;
    }

    return m_reg_file[reg];
}

void RegisterFile::write (RegisterFile::Register reg, uint data) {
    if (reg == x0) {
        std::cout << "Register x0 cannot be written to\n";
        return;
    }

    m_reg_file[reg] = data;
}
