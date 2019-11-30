//
// Created by matej on 11/19/19.
//

#include "stack.h"

Stack* Stack::instance = nullptr;

Stack::Stack () {}

void Stack::writeByte (unsigned int sp, unsigned char data) {
    unsigned int index = stack_pointer_to_index(sp);
    m_stack.at(index) = data;
}

void Stack::writeHalf (unsigned int sp, unsigned short data) {
    unsigned int index = stack_pointer_to_index(sp);
    m_stack.at(index) = data & 0x00FF;
    m_stack.at(index-1) = (data & 0xFF00) >> 8;
}

void Stack::writeWord (unsigned int sp, unsigned int data) {
    unsigned int index = stack_pointer_to_index(sp);
    m_stack.at(index) = data & 0x000000FF;
    m_stack.at(index-1) = (data & 0x0000FF00) >> 8;
    m_stack.at(index-2) = (data & 0x00FF0000) >> 16;
    m_stack.at(index-3) = (data & 0xFF000000) >> 24;
}

unsigned int Stack::stack_pointer_to_index (unsigned int sp) {
    return STACK_SIZE - int(sp);
}

unsigned char Stack::readByte (unsigned int sp) {
    unsigned int index = stack_pointer_to_index(sp);
    return m_stack.at(index);
}

unsigned short Stack::readHalf (unsigned int sp) {
    unsigned int index = stack_pointer_to_index(sp);
    unsigned short data = m_stack.at(index) + (m_stack.at(index+1) << 8);
    return data;
}

unsigned int Stack::readWord (unsigned int sp) {
    unsigned int index = stack_pointer_to_index(sp);
    unsigned int data = m_stack.at(index) + (m_stack.at(index+1) << 8) + (m_stack.at(index+2) << 18) + (m_stack.at(index+3) << 24);
    return data;
}

Stack *Stack::getInstance () {
    if (instance == nullptr) {
        instance = new Stack();
    }
    return instance;
}


