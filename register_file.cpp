//
// Created by matej on 10/10/19.
//

#include <iostream>
#include <iomanip>
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
#ifdef DEBUG
        std::cout << "Register x0 cannot be written to\n";
#endif
        return;
    }
    m_reg_file[reg] = data;
}

void RegisterFile::print_registers () {
    std::cout << "Register file:\n";
    for (unsigned long i = 0; i < m_reg_file.size(); i++) {
        std::cout << std::dec << "x" << i << " - " << m_reg_file[i] << "(" << int(m_reg_file[i]) << "), ";
        std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << m_reg_file[i] << "\n";
    }
}

RegisterFile *RegisterFile::getInstance () {
    if (instance == nullptr) {
        instance = new RegisterFile();
    }
    return instance;
}
