//
// Created by matej on 10/10/19.
//

#ifndef ISA_SIM_CPP_REGISTER_FILE_H
#define ISA_SIM_CPP_REGISTER_FILE_H


#include <array>

class RegisterFile {
private:
    std::array<uint, 32> m_reg_file;
public:
    enum Register {
        x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16,
        x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31
    };
    RegisterFile();
    void write (Register reg, uint data);
    uint read (Register reg);
    void print_registers ();
};


#endif //ISA_SIM_CPP_REGISTER_FILE_H