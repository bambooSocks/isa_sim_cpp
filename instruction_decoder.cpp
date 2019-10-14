//
// Created by matej on 10/14/19.
//

#include <iostream>
#include "instruction_decoder.h"

/**
 * Function decoding register-register arithmetic and logic instructions
 * @param inst  raw instruction
 */
void RegArithLogDecoder::decode (unsigned int inst) {
    r_inst_t decoder;
    decoder.inst = inst;

    // if bit 0 in funct7 is 0 execute basic arithmetic and logic instructions,
    // otherwise execute instructions from M extension.
    if (!(decoder.f.funct7 & 0x01u)) {
        // I extension
        i_extension_decode(decoder);
    } else {
        // M extension
        m_extension_decode(decoder);
    }


}

/**
 * Function decoding register-register arithmetic and logic instructions
 * for the base instructions in RISCV32I
 * @param inst  raw instruction
 */
void RegArithLogDecoder::i_extension_decode (RegArithLogDecoder::r_inst_t decoder) {
    switch (decoder.f.funct3) {
        case 0b000:
            // check the bit 6 in funct7
            if (decoder.f.funct7 & 0x20u == 0) {
                // ADD
            } else if (decoder.f.funct7 & 0x20u == 1) {
                // SUB
            } else {
                // error
            }
            break;
        case 0b001:
            // SLL
            break;
        case 0b010:
            // SLT
            break;
        case 0b011:
            // SLTU
            break;
        case 0b100:
            // XOR
            break;
        case 0b101:
            // check the bit 6 in funct7
            if (decoder.f.funct7 & 0x20u == 0) {
                // SRL
            } else if (decoder.f.funct7 & 0x20u == 1) {
                // SRA
            } else {
                // error
            }
            break;
        case 0b110:
            // OR
            break;
        case 0b111:
            // AND
            break;
        default:
            std::cerr << "Invalid funct3 while reg arith log I decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
}

/**
 * Function decoding register-register arithmetic and logic instructions
 * for the base instructions in RISCV32M
 * @param inst  raw instruction
 */
void RegArithLogDecoder::m_extension_decode (RegArithLogDecoder::r_inst_t decoder) {
    switch (decoder.f.funct3) {
        case 0b000:
            // MUL
            break;
        case 0b001:
            // MULH
            break;
        case 0b010:
            // MULHSU
            break;
        case 0b011:
            // MLHU
            break;
        case 0b100:
            // DIV
            break;
        case 0b101:
            // DIVU
            break;
        case 0b110:
            // REM
            break;
        case 0b111:
            // REMU
            break;
        default:
            std::cerr << "Invalid funct3 while reg arith log M decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
}

/**
 * Function decoding register-immediate arithmetic and logic instructions
 * @param inst  raw instruction
 */
void ImmArithLogDecoder::decode (unsigned int inst) {
    i_inst_t decoder;
    decoder.inst = inst;

    switch (decoder.f.funct3) {
        case 0b000:
            // ADDI
            break;
        case 0b001:
            // SLLI
            break;
        case 0b010:
            // SLTI
            break;
        case 0b011:
            // SLTIU
            break;
        case 0b100:
            // XORI
            break;
        case 0b101:
            // check the bit 10 in imm
            if (decoder.f.imm & 0x0400u == 0) {
                // SRL
            } else if (decoder.f.imm & 0x0400u == 1) {
                // SRA
            } else {
                // error
            }
            break;
        case 0b110:
            // ORI
            break;
        case 0b111:
            // ANDI
            break;
        default:
            std::cerr << "Invalid funct3 while imm arith log decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
}

/**
 * Function decoding load instructions
 * @param inst  raw instruction
 */
void LoadDecoder::decode (unsigned int inst) {
    i_inst_t decoder;
    decoder.inst = inst;

    switch (decoder.f.funct3) {
        case 0b000:
            // LB
            break;
        case 0b001:
            // LH
            break;
        case 0b010:
            // LW
            break;
        case 0b100:
            // LBU
            break;
        case 0b101:
            // LHU
            break;
        default:
            std::cerr << "Invalid funct3 while load decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
}

/**
 * Function decoding store instructions
 * @param inst  raw instruction
 */
void StoreDecoder::decode (unsigned int inst) {
    s_inst_t decoder;
    decoder.inst = inst;

    unsigned short imm = decoder.f.imm4_0 | (decoder.f.imm5_11 << 5);

    switch (decoder.f.funct3) {
        case 0b000:
            // SB
            break;
        case 0b001:
            // SH
            break;
        case 0b010:
            // SW
            break;
        default:
            std::cerr << "Invalid funct3 while store decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
}

/**
 * Function decoding branch instructions
 * @param inst  raw instruction
 */
void BranchDecoder::decode (unsigned int inst) {
    b_inst_t decoder;
    decoder.inst = inst;

    unsigned short imm = (decoder.f.imm4_1 << 1) | (decoder.f.imm5_10 << 5) |
                         (decoder.f.imm11 << 11) | (decoder.f.imm12 << 12);

    switch (decoder.f.funct3) {
        case 0b000:
            // BEQ
            break;
        case 0b001:
            // BNE
            break;
        case 0b100:
            // BLT
            break;
        case 0b101:
            // BGE
            break;
        case 0b110:
            // BLTU
            break;
        case 0b111:
            // BGEU
            break;
        default:
            std::cerr << "Invalid funct3 while branch decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
}

/**
 * Function decoding upper immediate instructions
 * @param inst  raw instruction
 */
void UpperImmDecoder::decode (unsigned int inst) {
    u_inst_t decoder;
    decoder.inst = inst;

    unsigned short imm = decoder.f.imm31_12 << 12;

    if (decoder.f.opcode == 0b0110111) {
        // LUI
    } else if (decoder.f.opcode == 0b0010111) {
        // AUIPC
    }
}

/**
 * Function decoding JAL instruction
 * @param inst  raw instruction
 */
void JumpLinkDecoder::decode (unsigned int inst) {
    j_inst_t decoder;
    decoder.inst = inst;

    unsigned short imm = (decoder.f.imm10_1 << 1) | (decoder.f.imm11 << 11) |
                         (decoder.f.imm19_12 << 12) | (decoder.f.imm20 << 20);

    // JAL
}

/**
 * Function decoding JALR instruction
 * @param inst  raw instruction
 */
void JumpLinkRegDecoder::decode (unsigned int inst) {
    i_inst_t decoder;
    decoder.inst = inst;

    //JALR
}
