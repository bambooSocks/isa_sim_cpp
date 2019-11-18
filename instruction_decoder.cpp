//
// Created by matej on 10/14/19.
//

#include <iostream>
#include <cmath>
#include "instruction_decoder.h"

/**
 * Function decoding register-register arithmetic and logic instructions
 * @param inst  raw instruction
 */
void RegArithLogDecoder::decode (unsigned int inst) {
    r_inst_t decoder{};
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
void RegArithLogDecoder::i_extension_decode (r_inst_t decoder) {
    rs1 = reg->read(decoder.f.rs1);
    rs2 = reg->read(decoder.f.rs2);

    switch (decoder.f.funct3) {
        case 0b000:
            // check the bit 6 in funct7
            if (!(decoder.f.funct7 & 0x20u)) {
                // ADD
                reg->write(decoder.f.rd, int(rs1) + int(rs2));
            } else {
                // SUB
                reg->write(decoder.f.rd, int(rs1) - int(rs2));
            }
            break;
        case 0b001:
            // SLL
            reg->write(decoder.f.rd, rs1 << rs2);
            break;
        case 0b010:
            // SLT
            reg->write(decoder.f.rd, int(rs1) < int(rs2));
            break;
        case 0b011:
            // SLTU
            reg->write(decoder.f.rd, rs1 < rs2);
            break;
        case 0b100:
            // XOR
            reg->write(decoder.f.rd, rs1 ^ rs2);
            break;
        case 0b101:
            // check the bit 6 in funct7
            if (!(decoder.f.funct7 & 0x20u)) {
                // SRL
                reg->write(decoder.f.rd, rs1 >> rs2);
            } else {
                // SRA
                reg->write(decoder.f.rd, int(rs1) / pow(2.,rs2));
            }
            break;
        case 0b110:
            // OR
            reg->write(decoder.f.rd, rs1 | rs2);
            break;
        case 0b111:
            // AND
            reg->write(decoder.f.rd, rs1 & rs2);
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
void RegArithLogDecoder::m_extension_decode (r_inst_t decoder) {
    long long temp;
    rs1 = reg->read(decoder.f.rs1);
    rs2 = reg->read(decoder.f.rs2);

    switch (decoder.f.funct3) {
        case 0b000:
            // MUL
            reg->write(decoder.f.rd, rs1 * rs2);
            break;
        case 0b001:
            // MULH
            temp = int(rs1) * int(rs2);
            reg->write(decoder.f.rd, temp >> 32);
            break;
        case 0b010:
            // MULHSU
            temp = int(rs1) * rs2;
            reg->write(decoder.f.rd, temp >> 32);
            break;
        case 0b011:
            // MLHU
            temp = rs1 * rs2;
            reg->write(decoder.f.rd, temp >> 32);
            break;
        case 0b100:
            // DIV
            reg->write(decoder.f.rd, int(rs1) / int(rs2));
            break;
        case 0b101:
            // DIVU
            reg->write(decoder.f.rd, rs1 / rs2);
            break;
        case 0b110:
            // REM
            reg->write(decoder.f.rd, int(rs1) % int(rs2));
            break;
        case 0b111:
            // REMU
            reg->write(decoder.f.rd, rs1 % rs2);
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
    i_inst_t decoder{};
    decoder.inst = inst;

    rs1 = reg->read(decoder.f.rs1);
    imm = decoder.f.imm;
    // sign-extend if negative
    if (imm & 0x800) {
        imm |= 0xFFFFF000;
    }

    switch (decoder.f.funct3) {
        case 0b000:
            // ADDI
            reg->write(decoder.f.rd, int(rs1) + int(imm));
            break;
        case 0b001:
            // SLLI
            reg->write(decoder.f.rd, rs1 << imm);
            break;
        case 0b010:
            // SLTI
            reg->write(decoder.f.rd, int(rs1) < int(imm));
            break;
        case 0b011:
            // SLTIU
            reg->write(decoder.f.rd, rs1 < imm);
            break;
        case 0b100:
            // XORI
            reg->write(decoder.f.rd, rs1 ^ imm);
            break;
        case 0b101:
            // check the bit 10 in imm
            if (!(decoder.f.imm & 0x0400u)) {
                // SRLI
                reg->write(decoder.f.rd, rs1 >> imm);
            } else {
                // SRAI
                reg->write(decoder.f.rd, int(rs1) / pow(2.,imm));
            }
            break;
        case 0b110:
            // ORI
            reg->write(decoder.f.rd, rs1 | imm);
            break;
        case 0b111:
            // ANDI
            reg->write(decoder.f.rd, rs1 & imm);
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
    i_inst_t decoder{};
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
    s_inst_t decoder{};
    decoder.inst = inst;

    imm = decoder.f.imm4_0 | (decoder.f.imm5_11 << 5u);

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
    b_inst_t decoder{};
    decoder.inst = inst;

    imm = (decoder.f.imm4_1 << 1u) | (decoder.f.imm5_10 << 5u) |
                         (decoder.f.imm11 << 11u) | (decoder.f.imm12 << 12u);
    // sign-extend if negative
    if (imm & 0x1000) {
        imm &= 0xFFFFE000;
    }

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
    u_inst_t decoder{};
    decoder.inst = inst;

    imm = int(decoder.f.imm31_12) << 12u & 0xFFFFF000u;

    if (decoder.f.opcode == 0b0110111) {
        // LUI
        reg->write(decoder.f.rd, imm);
    } else if (decoder.f.opcode == 0b0010111) {
        // AUIPC
        //TODO: figure out how to handle pc
//        reg->write(decoder.f.rd, (pc*4)+imm);
    }
}

/**
 * Function decoding JAL instruction
 * @param inst  raw instruction
 */
void JumpLinkDecoder::decode (unsigned int inst) {
    j_inst_t decoder{};
    decoder.inst = inst;

    imm = (decoder.f.imm10_1 << 1u) | (decoder.f.imm11 << 11u) |
          (decoder.f.imm19_12 << 12u) | (decoder.f.imm20 << 20u);

    // JAL
}

/**
 * Function decoding JALR instruction
 * @param inst  raw instruction
 */
void JumpLinkRegDecoder::decode (unsigned int inst) {
    i_inst_t decoder{};
    decoder.inst = inst;

    //temp
    imm = decoder.f.imm;

    //JALR
}

InstructionDecoder::InstructionDecoder () {
    reg = RegisterFile::getInstance();
    rs1 = 0;
    rs2 = 0;
    imm = 0;
}
