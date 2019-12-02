//
// Created by matej on 10/10/19.
//

#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include "register_file.h"

RegisterFile* RegisterFile::instance = nullptr;

/**
 * Register file constructor
 */
RegisterFile::RegisterFile () {
    for (unsigned int & i : m_reg_file) {
        i = 0;
    }
}

/**
 * Read register from register file
 * @param reg   Register number
 * @return      Data stored in the register
 */
unsigned int RegisterFile::read(RegisterFile::Register reg) {
    if (reg == x0) {
        return 0;
    }
    return m_reg_file[reg];
}

/**
 * Write data to register
 * @param reg   Register number
 * @param data  Data to be stored in register
 */
void RegisterFile::write (RegisterFile::Register reg, unsigned int data) {
    if (reg == x0) {
#ifdef DEBUG
        std::cout << "Register x0 cannot be written to\n";
#endif
        return;
    }
    m_reg_file[reg] = data;
}

/**
 * Print out the contents of Register File
 */
void RegisterFile::print_registers () {
    std::cout << "\033[1mRegister file:\033[0m\n";
    std::cout << "\033[1;31mRegister\033[0m    \033[1;33mHex\033[0m           \033[1;34mDec Unsigned(Dec Signed)\033[0m\n";
    for (unsigned long i = 0; i < m_reg_file.size(); i++) {

            std::cout << std::dec  << "x" << std::setfill('0') << std::setw(2) << i << "         ";
            std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << m_reg_file[i];
            std::cout << std::dec << "    " << m_reg_file[i] << "(" << int(m_reg_file[i]) << ")\n";

    }
}
/*          Red: 31, BrightRed: 91, Green: 32, BrightGreen: 92, Blue: 34, BrightBlue: 34
            std::cout << std::dec  << "\033[91mx" << std::setfill('0') << std::setw(2) << i << "\033[0m         ";
            std::cout << "\033[92m0x" << std::setfill('0') << std::setw(8) << std::hex << m_reg_file[i] << "\033[0m";
            std::cout << std::dec << "    \033[94m" << m_reg_file[i] << "(" << int(m_reg_file[i]) << ")\033[0m\n";
 */

/**
 * Gets singleton instance of the Register File
 * @return  instance of singleton
 */
RegisterFile *RegisterFile::getInstance () {
    if (instance == nullptr) {
        instance = new RegisterFile();
    }
    return instance;
}

/**
 * Dumps register file into a binary output.res file
 */
void RegisterFile::dump_registers () {
    std::filesystem::path path{"./output.res"};

    std::ofstream ofs(path);
    auto buffer = reinterpret_cast<char *>(m_reg_file.data());

    ofs.write(buffer, 32*4);
    ofs.close();
}

