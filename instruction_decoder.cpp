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
unsigned int RegArithLogDecoder::decode (unsigned int pc, unsigned int inst) {
    r_inst_t decoder{};
    decoder.inst = inst;

    // if bit 0 in funct7 is 0 execute basic arithmetic and logic instructions,
    // otherwise execute instructions from M extension.
    if (!(decoder.f.funct7 & 0x01u)) {
        // I extension
        return i_extension_decode(pc, decoder);
    } else {
        // M extension
        return m_extension_decode(pc, decoder);
    }
}

/**
 * Function decoding register-register arithmetic and logic instructions
 * for the base instructions in RISCV32I
 * @param inst  raw instruction
 */
unsigned int RegArithLogDecoder::i_extension_decode (unsigned int pc, r_inst_t decoder) {
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
    return pc+4;
}

/**
 * Function decoding register-register arithmetic and logic instructions
 * for the base instructions in RISCV32M
 * @param inst  raw instruction
 */
unsigned int RegArithLogDecoder::m_extension_decode (unsigned int pc, r_inst_t decoder) {
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
    return pc+4;
}

/**
 * Function decoding register-immediate arithmetic and logic instructions
 * @param inst  raw instruction
 */
unsigned int ImmArithLogDecoder::decode (unsigned int pc, unsigned int inst) {
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
    return pc+4;
}

/**
 * Function decoding load instructions
 * @param inst  raw instruction
 */
unsigned int LoadDecoder::decode (unsigned int pc, unsigned int inst) {
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
    return pc+4;
}

/**
 * Function decoding store instructions
 * @param inst  raw instruction
 */
unsigned int StoreDecoder::decode (unsigned int pc, unsigned int inst) {
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
    return pc+4;
}

/**
 * Function decoding branch instructions
 * @param inst  raw instruction
 */
unsigned int BranchDecoder::decode (unsigned int pc, unsigned int inst) {
    b_inst_t decoder{};
    decoder.inst = inst;

    rs1 = decoder.f.rs1;
    rs2 = decoder.f.rs2;
    imm = (decoder.f.imm4_1 << 1u) | (decoder.f.imm5_10 << 5u) |
                         (decoder.f.imm11 << 11u) | (decoder.f.imm12 << 12u);
    // sign-extend if negative
    if (imm & 0x1000) {
        imm |= 0xFFFFE000;
    }

    switch (decoder.f.funct3) {
        case 0b000:
            // BEQ
            if (rs1 == rs2) {
                return pc + imm;
            }
            break;
        case 0b001:
            // BNE
            if (rs1 != rs2) {
                return pc + imm;
            }
            break;
        case 0b100:
            // BLT
            if (int(rs1) < int(rs2)) {
                return pc + imm;
            }
            break;
        case 0b101:
            // BGE
            if (int(rs1) >= int(rs2)) {
                return pc + imm;
            }
            break;
        case 0b110:
            // BLTU
            if (rs1 < rs2) {
                return pc + imm;
            }
            break;
        case 0b111:
            // BGEU
            if (rs1 >= rs2) {
                return pc + imm;
            }
            break;
        default:
            std::cerr << "Invalid funct3 while branch decoding: " << decoder.f.funct3 << "\n";
            exit(1);
    }
    return pc+4;
}

/**
 * Function decoding upper immediate instructions
 * @param inst  raw instruction
 */
unsigned int UpperImmDecoder::decode (unsigned int pc, unsigned int inst) {
    u_inst_t decoder{};
    decoder.inst = inst;

    imm = int(decoder.f.imm31_12) << 12u & 0xFFFFF000u;

    if (decoder.f.opcode == 0b0110111) {
        // LUI
        reg->write(decoder.f.rd, imm);
    } else if (decoder.f.opcode == 0b0010111) {
        // AUIPC
        reg->write(decoder.f.rd, pc + imm);
    }
    return pc+4;
}

/**
 * Function decoding JAL instruction
 * @param inst  raw instruction
 */
unsigned int JumpLinkDecoder::decode (unsigned int pc, unsigned int inst) {
    j_inst_t decoder{};
    decoder.inst = inst;

    // TODO: test
    imm = (decoder.f.imm10_1 << 1u) | (decoder.f.imm11 << 11u) |
          (decoder.f.imm19_12 << 12u) | (decoder.f.imm20 << 20u);
    // sign-extend if negative
    if (imm & 0x100000) {
        imm |= 0xFFE00000;
    }

    reg->write(decoder.f.rd, pc+4);

    // JAL
    return pc + imm;
}

/**
 * Function decoding JALR instruction
 * @param inst  raw instruction
 */
unsigned int JumpLinkRegDecoder::decode (unsigned int pc, unsigned int inst) {
    i_inst_t decoder{};
    decoder.inst = inst;

    rs1 = reg->read(decoder.f.rs1);
    imm = decoder.f.imm;
    // sign-extend if negative
    if (imm & 0x800) {
        imm |= 0xFFFFF000;
    }

    // The target address is obtained by adding the sign-extended
    // 12-bit I-immediate to the register rs1, then setting
    // the least-significant bit of the result to zero.
    unsigned int offset = (rs1 + imm) & 0xFFFFFFFEu;

    reg->write(decoder.f.rd, pc+4);

    //JALR
    return pc + offset;
}

InstructionDecoder::InstructionDecoder () {
    reg = RegisterFile::getInstance();
    rs1 = 0;
    rs2 = 0;
    imm = 0;
}

unsigned int EcallDecoder::decode(unsigned int pc, unsigned int inst) {
    i_inst_t decoder{};
    decoder.inst = inst;

    //temp
    rs1 = reg->read(decoder.f.rs1);
    imm = decoder.f.imm;

    //We need only check immediate that it is 0b000000000000 (rather than 0b000000000001), in order to find ecall
    if (imm == 0 && decoder.f.funct3 == 0) {
        switch (reg->read(RegisterFile::x10)) {
            case 10: // exit
                // terminate correctly
                // print registers
                break;
            case 17: // exit2
                // terminate with error
                // print registers
                break;
            default:
                std::cerr << "Unsupported instruction\r\n";
                exit(1);
        }
    } else {
        std::cerr << "Unsupported instruction\r\n";
        exit(1);
    }

    // Should I implement this somehow, to indicate we are at the end of file,
    // in order to jump out of the main while-loop
    //sim.executeInstruction() = EXEC_EOF

    //Ecall
    // FIXME: Should it return program counter or just 0??
    return -1;
}
