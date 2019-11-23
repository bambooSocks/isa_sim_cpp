//
// Created by matej on 10/10/19.
//

#include <iostream>
#include "register_file.h"

RegisterFile* RegisterFile::instance = nullptr;

RegisterFile::RegisterFile () {
    for (int i = 0; i < m_reg_file.size(); i++) {
        m_reg_file[i] = 0;
    }
}

unsigned int RegisterFile::read(RegisterFile::Register reg) {
    if (reg == x0) {
        return 0;
    }
    return m_reg_file[reg];
}

void RegisterFile::write (RegisterFile::Register reg, unsigned int data) {
    if (reg == x0) {
        std::cout << "Register x0 cannot be written to\n";
        return;
    }
    m_reg_file[reg] = data;
}

void RegisterFile::print_registers () {
    std::cout << "Register file:\n";
    for (unsigned long i = 0; i < m_reg_file.size(); i++) {
        std::cout << "x" << i << " - " << m_reg_file[i] << "\n";
    }
}

RegisterFile *RegisterFile::getInstance () {
    if (instance == nullptr) {
        instance = new RegisterFile();
    }
    return instance;
}
