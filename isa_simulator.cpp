//
// Created by matej on 10/14/19.
//

#include <filesystem>
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include "isa_simulator.h"

/**
 * ISA Simulator constructor: initializes the objects and the opcode map
 */
ISA_Simulator::ISA_Simulator () {
    pc = 0;
    registerFile = RegisterFile::getInstance();
    term = new Termination();
    // setup the opcode lookup map
    opcode_map.insert({0b0110011, new RegArithLogDecoder});
    opcode_map.insert({0b0010011, new ImmArithLogDecoder});
    opcode_map.insert({0b0000011, new LoadDecoder});
    opcode_map.insert({0b0100011, new StoreDecoder});
    opcode_map.insert({0b1100011, new BranchDecoder});
    opcode_map.insert({0b0110111, new UpperImmDecoder});
    opcode_map.insert({0b0010111, new UpperImmDecoder});
    opcode_map.insert({0b1101111, new JumpLinkDecoder});
    opcode_map.insert({0b1100111, new JumpLinkRegDecoder});
    opcode_map.insert({0b1110011, new EcallDecoder});
}

/**
 * Function for loading the binary file and converting it into
 * a standard vector of integers
 * @param filepath  the path to the binary file
 * @return          true if successful otherwise false
 */
bool ISA_Simulator::loadFile (const char *filepath) {
    std::ifstream file;

    if (!std::filesystem::exists(filepath) ||
        std::filesystem::is_directory(filepath)) {
        std::cerr << "Not a valid file\n";
        return false;
    }

    file.open(filepath);
    std::string lines;

    if (file.is_open()) {
        char c;
        while (file.get(c)) {
            lines.append(&c,1);
        }
        file.close();
    }

    auto *temp = reinterpret_cast<unsigned int*>(lines.data());
    inst_mem.insert(inst_mem.end(), &temp[0], &temp[lines.length() / 4]);
    return true;
}

/**
 * Fetch and execute next instruction from the instruction memory
 * @return  false if EOF is reached otherwise true
 */
exec_result_t ISA_Simulator::executeInstruction () {
    unsigned char opcode = 0;
    try {
        // fetch instruction
        unsigned int inst = inst_mem.at(pc / 4);
        // decode instruction and execute
        opcode = inst & 0x0000007Fu;

        // execute instruction and update pc
        pc = opcode_map.at(opcode)->decode(pc, inst);

#ifdef DEBUG
        std::cout << "\nProgram counter: " << std::dec << pc << "\n";
        registerFile->print_registers();
#endif

    } catch (const std::out_of_range& e) {
        std::string exception = e.what();
        // distinguish between individual exceptions
        if (exception.find("vector::_M_range_check") != std::string::npos) {
            // out of range of inst_mem
            //TODO: test this
            if (pc == inst_mem.size() * 4 + 4) {
                // one further than the size => EOF
                term->terminate("End of file reached", 0);
                return EXEC_EOF;
            } else {
                //wrong address (pc)
                term->terminate("Wrong instruction address: pc = " + std::to_string(pc), -1);
                return EXEC_ERROR;
            }
        } else if (exception.find("map::at") != std::string::npos) {
            // wrong opcode
            term->terminate("Wrong opcode or not implemented instruction: opcode="
                               + std::bitset<7>(opcode).to_string(), -1);
            return EXEC_ERROR;
        } else {
            // other error
            std::string msg = "Unknown error occurred: ";
            msg += e.what();
            term->terminate(msg, -1);
            return EXEC_ERROR;
        }
    }
    return EXEC_OK;
}

