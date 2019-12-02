// instruction_decoder.cpp
// Matej Majtan (s184457) & Søren Tønnesen (s180381)
// 02-12-2019

#include <iostream>
#include <string>
#include "instruction_decoder.h"

/**
 * InstructionDecoder base constructor
 */
InstructionDecoder::InstructionDecoder () {
    term = new Termination();
    reg = RegisterFile::getInstance();
    stack = Stack::getInstance();
    rs1 = 0;
    rs2 = 0;
    imm = 0;
}

/**
* Function decoding register-register arithmetic and logic instructions
* @param pc    program counter
* @param inst  raw instruction
* @return      new program counter
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
 * @param pc        program counter
 * @param decoder   decoder union
 * @return          new program counter
 */
unsigned int RegArithLogDecoder::i_extension_decode (unsigned int pc, r_inst_t decoder) {
    rs1 = reg->read(decoder.f.rs1);
    rs2 = reg->read(decoder.f.rs2);

    std::string i_name;

    switch (decoder.f.funct3) {
        case 0b000:
            // check the bit 6 in funct7
            if (!(decoder.f.funct7 & 0x20u)) {
                // ADD
                i_name = "add";
                reg->write(decoder.f.rd, int(rs1) + int(rs2));
            } else {
                // SUB
                i_name = "sub";
                reg->write(decoder.f.rd, int(rs1) - int(rs2));
            }
            break;
        case 0b001:
            // SLL
            i_name = "sll";
            reg->write(decoder.f.rd, rs1 << rs2);
            break;
        case 0b010:
            // SLT
            i_name = "slt";
            reg->write(decoder.f.rd, int(rs1) < int(rs2));
            break;
        case 0b011:
            // SLTU
            i_name = "sltu";
            reg->write(decoder.f.rd, rs1 < rs2);
            break;
        case 0b100:
            // XOR
            i_name = "xor";
            reg->write(decoder.f.rd, rs1 ^ rs2);
            break;
        case 0b101:
            // check the bit 6 in funct7
            if (!(decoder.f.funct7 & 0x20u)) {
                // SRL
                i_name = "srl";
                reg->write(decoder.f.rd, rs1 >> rs2);
            } else {
                // SRA
                i_name = "sra";
                reg->write(decoder.f.rd, int(rs1) >> rs2);
            }
            break;
        case 0b110:
            // OR
            i_name = "or";
            reg->write(decoder.f.rd, rs1 | rs2);
            break;
        case 0b111:
            // AND
            i_name = "and";
            reg->write(decoder.f.rd, rs1 & rs2);
            break;
        default:
            term->terminate("Invalid funct3 while decoding register-register arithemtic or logical instruction (I): "
                            + std::to_string(decoder.f.funct3) + "\n", 1);

    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rd) + ", x" + std::to_string(decoder.f.rs1) + ", x" + std::to_string(decoder.f.rs2) + "\r\n";
#endif
    return pc+4;
}

/**
 * Function decoding register-register arithmetic and logic instructions
 * for the base instructions in RISCV32M
 * @param pc        program counter
 * @param decoder   decoder union
 * @return          new program counter
 */
unsigned int RegArithLogDecoder::m_extension_decode (unsigned int pc, r_inst_t decoder) {
    int32_t s_rd, s_rs1, s_rs2;
    uint64_t u_rd, u_rs1, u_rs2;

    rs1 = reg->read(decoder.f.rs1);
    rs2 = reg->read(decoder.f.rs2);

    std::string i_name;

    switch (decoder.f.funct3) {
        case 0b000:
            // MUL
            i_name = "mul";
            reg->write(decoder.f.rd, rs1 * rs2);
            break;
        case 0b001:
            // MULH - not tested yet
            i_name = "mulh";
            s_rd = int32_t((int64_t(rs1) * int64_t(rs2)) >> 32);
            reg->write(decoder.f.rd, s_rd);
            break;
        case 0b010:
            // MULHSU - not tested yet
            i_name = "mulhsu";
            u_rd = uint32_t((int64_t(rs1) * uint64_t(rs2)) >> 32);
            reg->write(decoder.f.rd, u_rd >> 32);
            break;
        case 0b011:
            // MLHU
            i_name = "mlhu";
            u_rs1 = rs1;
            u_rs2 = rs2;
            u_rd = u_rs1 * u_rs2;
            reg->write(decoder.f.rd, u_rd >> 32);
            break;
        case 0b100:
            // DIV
            i_name = "div";
            if (rs2 == 0) {
                reg->write(decoder.f.rd, -1);
            } else if (rs1 == 0x80000000 && rs2 == 0xFFFFFFFF) {
                reg->write(decoder.f.rd, 0x80000000);
            } else {
                reg->write(decoder.f.rd, int(rs1) / int(rs2));
            }
            break;
        case 0b101:
            // DIVU
            i_name = "divu";
            if (rs2 == 0) {
                reg->write(decoder.f.rd, rs1); // doesn't make sense
            } else {
                reg->write(decoder.f.rd, rs1 / rs2);
            }
            break;
        case 0b110:
            // REM
            i_name = "rem";
            if (rs2 == 0) {
                reg->write(decoder.f.rd, rs1);
            } else if (rs1 == 0x80000000 && rs2 == 0xFFFFFFFF) {
                reg->write(decoder.f.rd, 0);
            } else {
                reg->write(decoder.f.rd, int(rs1) % int(rs2));
            }
            break;
        case 0b111:
            // REMU
            i_name = "remu";
            if (rs2 == 0) {
                reg->write(decoder.f.rd, rs1);
            } else {
                reg->write(decoder.f.rd, rs1 % rs2);
            }
            break;
        default:
            term->terminate("Invalid funct3 while decoding register-register arithemtic or logical instruction (M): "
                            + std::to_string(decoder.f.funct3) + "\n", 1);
    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rd) + ", x" + std::to_string(decoder.f.rs1) + ", x" + std::to_string(decoder.f.rs2) + "\r\n";
#endif
    return pc+4;
}

/**
* Function decoding register-immediate arithmetic and logic instructions
* @param pc    program counter
* @param inst  raw instruction
* @return      new program counter
*/
unsigned int ImmArithLogDecoder::decode (unsigned int pc, unsigned int inst) {
    i_inst_t decoder{};
    decoder.inst = inst;

    std::string i_name;

    rs1 = reg->read(decoder.f.rs1);
    imm = decoder.f.imm;
    // sign-extend if negative
    if (imm & 0x800) {
        imm |= 0xFFFFF000;
    }

    switch (decoder.f.funct3) {
        case 0b000:
            // ADDI
            i_name = "addi";
            reg->write(decoder.f.rd, int(rs1) + int(imm));
            break;
        case 0b001:
            // SLLI
            i_name = "slli";
            reg->write(decoder.f.rd, rs1 << imm);
            break;
        case 0b010:
            // SLTI
            i_name = "slti";
            reg->write(decoder.f.rd, int(rs1) < int(imm));
            break;
        case 0b011:
            // SLTIU
            i_name = "sltiu";
            reg->write(decoder.f.rd, rs1 < imm);
            break;
        case 0b100:
            // XORI
            i_name = "xori";
            reg->write(decoder.f.rd, rs1 ^ imm);
            break;
        case 0b101:
            // check the bit 10 in imm
            if (!(decoder.f.imm & 0x0400u)) {
                // SRLI
                i_name = "srli";
                reg->write(decoder.f.rd, rs1 >> imm);
            } else {
                // SRAI
                i_name = "srai";
                imm &= 0x1F;
                reg->write(decoder.f.rd, int(rs1) >> imm);
            }
            break;
        case 0b110:
            // ORI
            i_name = "ori";
            reg->write(decoder.f.rd, rs1 | imm);
            break;
        case 0b111:
            // ANDI
            i_name = "andi";
            reg->write(decoder.f.rd, rs1 & imm);
            break;
        default:
            term->terminate("Invalid funct3 while decoding register-immediate arithemtic or logical instruction: "
                            + std::to_string(decoder.f.funct3) + "\n", 1);
    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rd) + ", x" + std::to_string(decoder.f.rs1) + ", " + std::to_string(int(imm)) + "\r\n";
#endif
    return pc+4;
}

/**
 * Function decoding load instructions
 * @param pc    program counter
 * @param inst  raw instruction
 * @return      new program counter
 */
unsigned int LoadDecoder::decode (unsigned int pc, unsigned int inst) {
    i_inst_t decoder{};
    decoder.inst = inst;
    unsigned int data;

    std::string i_name;

    rs1 = reg->read(decoder.f.rs1);
    imm = decoder.f.imm;
    // sign-extend if negative
    if (imm & 0x800) {
        imm |= 0xFFFFF000;
    }
    unsigned int sp = int(rs1) + int(imm);

    switch (decoder.f.funct3) {
        case 0b000:
            // LB
            i_name = "lb";
            data = stack->readByte(sp);
            // sign-extend if negative
            if (data & 0x80) {
                data |= 0xFFFFFF00;
            }
            reg->write(decoder.f.rd, data);
            break;
        case 0b001:
            // LH
            i_name = "lh";
            data = stack->readHalf(sp);
            // sign-extend if negative
            if (data & 0x8000) {
                data |= 0xFFFF0000;
            }
            reg->write(decoder.f.rd, data);
            break;
        case 0b010:
            // LW
            i_name = "lw";
            data = stack->readWord(sp);
            reg->write(decoder.f.rd, data);
            break;
        case 0b100:
            // LBU
            i_name = "lbu";
            data = stack->readByte(sp);
            reg->write(decoder.f.rd, data);
            break;
        case 0b101:
            // LHU
            i_name = "lhu";
            data = stack->readHalf(sp);
            reg->write(decoder.f.rd, data);
            break;
        default:
            term->terminate("Invalid funct3 while decoding load instruction: "
                            + std::to_string(decoder.f.funct3) + "\n", 1);
    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rd) + ", " + std::to_string(int(imm)) + "(x" + std::to_string(decoder.f.rs1) + ")\r\n";
#endif
    return pc+4;
}

/**
 * Function decoding store instructions
 * @param pc    program counter
 * @param inst  raw instruction
 * @return      new program counter
 */
unsigned int StoreDecoder::decode (unsigned int pc, unsigned int inst) {
    s_inst_t decoder{};
    decoder.inst = inst;

    std::string i_name;

    rs1 = reg->read(decoder.f.rs1);
    rs2 = reg->read(decoder.f.rs2);
    imm = decoder.f.imm4_0 | (decoder.f.imm5_11 << 5u);
    // sign-extend if negative
    if (imm & 0x800) {
        imm |= 0xFFFFF000;
    }
    unsigned int sp = int(rs1) + int(imm);

    switch (decoder.f.funct3) {
        case 0b000:
            // SB
            i_name = "sb";
            stack->writeByte(sp, rs2);
            break;
        case 0b001:
            // SH
            i_name = "sh";
            stack->writeHalf(sp, rs2);
            break;
        case 0b010:
            // SW
            i_name = "sw";
            stack->writeWord(sp, rs2);
            break;
        default:
            term->terminate("Invalid funct3 while decoding store instruction: "
                            + std::to_string(decoder.f.funct3) + "\n", 1);
    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rs2) + ", " + std::to_string(int(imm)) + "(x" + std::to_string(decoder.f.rs1) + ")\r\n";
#endif
    return pc+4;
}

/**
* Function decoding branch instructions
* @param pc    program counter
* @param inst  raw instruction
* @return      new program counter
*/
unsigned int BranchDecoder::decode (unsigned int pc, unsigned int inst) {
    b_inst_t decoder{};
    decoder.inst = inst;

    std::string i_name;

    rs1 = reg->read(decoder.f.rs1);
    rs2 = reg->read(decoder.f.rs2);
    imm = (decoder.f.imm4_1 << 1u) | (decoder.f.imm5_10 << 5u) |
                         (decoder.f.imm11 << 11u) | (decoder.f.imm12 << 12u);
    // sign-extend if negative
    if (imm & 0x1000) {
        imm |= 0xFFFFE000;
    }

    switch (decoder.f.funct3) {
        case 0b000:
            // BEQ
            i_name = "beq";
            if (rs1 == rs2) {
                return pc + int(imm);
            }
            break;
        case 0b001:
            // BNE
            i_name = "bne";
            if (rs1 != rs2) {
                return pc + int(imm);
            }
            break;
        case 0b100:
            // BLT
            i_name = "blt";
            if (int(rs1) < int(rs2)) {
                return pc + int(imm);
            }
            break;
        case 0b101:
            // BGE
            i_name = "bge";
            if (int(rs1) >= int(rs2)) {
                return pc + int(imm);
            }
            break;
        case 0b110:
            // BLTU
            i_name = "bltu";
            if (rs1 < rs2) {
                return pc + int(imm);
            }
            break;
        case 0b111:
            // BGEU
            i_name = "bgeu";
            if (rs1 >= rs2) {
                return pc + int(imm);
            }
            break;
        default:
            term->terminate("Invalid funct3 while decoding branch instruction: "
                            + std::to_string(decoder.f.funct3) + "\n", 1);
    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rs1) + ", x" + std::to_string(decoder.f.rs2) + ", " + std::to_string(int(imm)) + "\r\n";
#endif
    return pc+4;
}

/**
* Function decoding upper immediate instructions
* @param pc    program counter
* @param inst  raw instruction
* @return      new program counter
*/
unsigned int UpperImmDecoder::decode (unsigned int pc, unsigned int inst) {
    u_inst_t decoder{};
    decoder.inst = inst;

    std::string i_name;

    imm = int(decoder.f.imm31_12) << 12u & 0xFFFFF000u;

    if (decoder.f.opcode == 0b0110111) {
        // LUI
        i_name = "lui";
        reg->write(decoder.f.rd, imm);
    } else if (decoder.f.opcode == 0b0010111) {
        // AUIPC
        i_name = "auipc";
        reg->write(decoder.f.rd, pc + imm);
    }
#ifdef DEBUG
    std::cout << i_name + " x" + std::to_string(decoder.f.rd) + ", " + std::to_string(int(imm)) + "\r\n";
#endif
    return pc+4;
}

/**
* Function decoding JAL instructions
* @param pc    program counter
* @param inst  raw instruction
* @return      new program counter
*/
unsigned int JumpLinkDecoder::decode (unsigned int pc, unsigned int inst) {
    j_inst_t decoder{};
    decoder.inst = inst;

    // TODO: test
    imm = (decoder.f.imm19_12 << 12u) | (decoder.f.imm11 << 11u) |
          (decoder.f.imm10_1 << 1u) | (decoder.f.imm20 << 20u);
    // sign-extend if negative
    if (imm & 0x100000) {
        imm |= 0xFFE00000;
    }

    reg->write(decoder.f.rd, pc+4);

    // JAL
#ifdef DEBUG
    std::cout << "jal x" + std::to_string(decoder.f.rd) + ", " + std::to_string(int(imm)) + "\r\n";
#endif
    return pc + imm;
}

/**
* Function decoding JALR instructions
* @param pc    program counter
* @param inst  raw instruction
* @return      new program counter
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
#ifdef DEBUG
    std::cout << "jalr x" + std::to_string(decoder.f.rd) + ", x" + std::to_string(decoder.f.rs1) + ", " + std::to_string(int(imm)) + "\r\n";
#endif
    return offset;
}

/**
 * Function decoding environmental call instructions
 * @param pc    program counter
 * @param inst  raw instruction
 * @return      new program counter
 */
unsigned int EcallDecoder::decode(unsigned int pc, unsigned int inst) {
    i_inst_t decoder{};
    decoder.inst = inst;

#ifdef DEBUG
    std::cout << "ecall\r\n";
#endif

    //temp
    rs1 = reg->read(decoder.f.rs1);
    imm = decoder.f.imm;

    //We need only check immediate that it is 0b000000000000 (rather than 0b000000000001), in order to find ecall
    if (imm == 0 && decoder.f.funct3 == 0) {
        switch (reg->read(RegisterFile::x10)) {
            case 10: // exit
                term->terminate("Ecall 10 reached", 0);
                break;
            case 17: // exit2
                term->terminate("Ecall 12 reached - exit code: "
                                + std::to_string(reg->read(RegisterFile::x11)) , 0);
                break;
            default:
                term->terminate("Unsupported instruction", 1);
        }
    } else {
        term->terminate("Unsupported instruction", 1);
    }

    return -1;
}
