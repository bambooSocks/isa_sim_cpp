// instruction_decoder.h
// Matej Majtan (s184457) & Søren Tønnesen (s180381)
// 02-12-2019

#ifndef ISA_SIM_CPP_INSTRUCTION_DECODER_H
#define ISA_SIM_CPP_INSTRUCTION_DECODER_H

#include "register_file.h"
#include "stack.h"
#include "termination.h"

/**
 * Instruction type decoders
 */

union r_inst_t {
    unsigned int inst;
    struct {
        unsigned int opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned int funct3: 3;
        RegisterFile::Register rs1: 5;
        RegisterFile::Register rs2: 5;
        unsigned int funct7: 7;
    } f; // fields
};

union i_inst_t {
    unsigned int inst;
    struct {
        unsigned int opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned int funct3: 3;
        RegisterFile::Register rs1: 5;
        unsigned int imm: 12;
    } f; // fields
};

union s_inst_t {
    unsigned int inst;
    struct {
        unsigned int opcode: 7;
        unsigned int imm4_0: 5;
        unsigned int funct3: 3;
        RegisterFile::Register rs1: 5;
        RegisterFile::Register rs2: 5;
        unsigned int imm5_11: 7;
    } f; // fields
};

union b_inst_t {
    unsigned int inst;
    struct {
        unsigned int opcode: 7;
        unsigned int imm11: 1;
        unsigned int imm4_1: 4;
        unsigned int funct3: 3;
        RegisterFile::Register rs1: 5;
        RegisterFile::Register rs2: 5;
        unsigned int imm5_10: 6;
        unsigned int imm12: 1;
    } f; // fields
};

union u_inst_t {
    unsigned int inst;
    struct {
        unsigned int opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned int imm31_12: 20;
    } f; // fields
};

union j_inst_t {
    unsigned int inst;
    struct {
        unsigned int opcode: 7;
        RegisterFile::Register rd: 5;
        unsigned int imm19_12: 8;
        unsigned int imm11: 1;
        unsigned int imm10_1: 10;
        unsigned int imm20: 1;
    } f; // fields
};

/**
 * Interface for instruction decoder
 */
class InstructionDecoder {
protected:
    RegisterFile *reg;
    Stack *stack;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int imm;
    Termination *term;
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

/**
 * Ecall instruction decoder
 */
class EcallDecoder : public InstructionDecoder {
public:
    unsigned int decode (unsigned int pc, unsigned int inst) override;
};

#endif //ISA_SIM_CPP_INSTRUCTION_DECODER_H
