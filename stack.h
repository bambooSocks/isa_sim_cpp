//
// Created by matej on 11/19/19.
//

#ifndef ISA_SIM_CPP_STACK_H
#define ISA_SIM_CPP_STACK_H

#include <array>

#define STACK_SIZE 0x100000

class Stack {
private:
    //TODO: ask about stack size - maybe static
    std::array<unsigned char, STACK_SIZE> m_stack;
    static Stack *instance;

    static unsigned int stack_pointer_to_index (unsigned int sp);
    Stack ();

public:
    static Stack *getInstance ();
    void writeByte (unsigned int sp, unsigned char data);
    void writeHalf (unsigned int sp, unsigned short data);
    void writeWord (unsigned int sp, unsigned int data);

    unsigned char  readByte (unsigned int sp);
    unsigned short readHalf (unsigned int sp);
    unsigned int   readWord (unsigned int sp);
};


#endif //ISA_SIM_CPP_STACK_H
