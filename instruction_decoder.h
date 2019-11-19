//
// Created by matej on 10/14/19.
//

#ifndef ISA_SIM_CPP_INSTRUCTION_DECODER_H
#define ISA_SIM_CPP_INSTRUCTION_DECODER_H

#include "register_file.h"

/**
 * Instruction type decoders
 */

union r_inst_t {
    unsigned int inst;
    struct {
        unsigned char opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned char funct3: 3;
        RegisterFile::Register rs1: 5;
        RegisterFile::Register rs2: 5;
        unsigned char funct7: 7;
    } f; // fields
};

union i_inst_t {
    unsigned int inst;
    struct {
        unsigned char opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned char funct3: 3;
        RegisterFile::Register rs1: 5;
        unsigned short imm: 12;
    } f; // fields
};

union s_inst_t {
    unsigned int inst;
    struct {
        unsigned char opcode: 7;
        unsigned char imm4_0: 5;
        unsigned char funct3: 3;
        RegisterFile::Register rs1: 5;
        RegisterFile::Register rs2: 5;
        unsigned char imm5_11: 7;
    } f; // fields
};

union b_inst_t {
    unsigned int inst;
    struct {
        unsigned char opcode: 7;
        unsigned char imm11: 1;
        unsigned char imm4_1: 4;
        unsigned char funct3: 3;
        RegisterFile::Register rs1: 5;
        RegisterFile::Register rs2: 5;
        unsigned char imm5_10: 6;
        unsigned char imm12: 1;
    } f; // fields
};

union u_inst_t {
    unsigned int inst;
    struct {
        unsigned char opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned int imm31_12: 20;
    } f; // fields
};

union j_inst_t {
    unsigned int inst;
    struct {
        unsigned char opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned char imm19_12: 8;
        unsigned char imm11: 1;
        unsigned short imm10_1: 10;
        unsigned char imm20: 1;
    } f; // fields
};

/**
 * Interface for instruction decoder
 */
class InstructionDecoder {
protected:
    RegisterFile *reg;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int imm;
public:
    InstructionDecoder ();
    virtual unsigned int decode (unsigned int pc, unsigned int inst) = 0;
};

/**
 * Register-register Arithmetic and Logic instruction decoder
 */
class RegArithLogDecoder : public InstructionDecoder {
private:
    unsigned int i_extension_decode (unsigned int pc, r_inst_t decoder);
    unsigned int m_extension_decode (unsigned int pc, r_inst_t decoder);
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Register-immediate Arithmetic and Logic instruction decoder
 */
class ImmArithLogDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Load instruction decoder
 */
class LoadDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Store instruction decoder
 */
class StoreDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Branch instruction decoder
 */
class BranchDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Upper immediate instruction decoder
 */
class UpperImmDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Jump and link instruction decoder
 */
class JumpLinkDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

/**
 * Jump and link via register instruction decoder
 */
class JumpLinkRegDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

#endif //ISA_SIM_CPP_INSTRUCTION_DECODER_H
