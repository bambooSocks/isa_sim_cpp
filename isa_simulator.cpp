//
// Created by matej on 10/14/19.
//

#include <filesystem>
#include <iostream>
#include <fstream>
#include <bitset>
#include "isa_simulator.h"


ISA_Simulator::ISA_Simulator () {
    pc = 0;
    registerFile = RegisterFile::getInstance();
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
        std::string line;
        while (getline (file, line)) {
            lines.append(line);
        }
        file.close();
    }

    auto *temp = reinterpret_cast<unsigned int*>(lines.data());
    raw_insts.insert(raw_insts.end(), &temp[0], &temp[lines.length() / 4]);
    return true;
}

/**
 *
 * @return  false if EOF is reached otherwise true
 */
exec_result_t ISA_Simulator::executeInstruction () {
    unsigned char opcode;
    try {
        // fetch instruction
        unsigned int inst = raw_insts.at(pc);
        // decode instruction and execute
        opcode = inst & 0x0000007Fu;
        opcode_map.at(opcode)->decode(inst);

        //TODO: branches and jumps!!!

        // just for debugging
        std::cout << "Program counter: " << pc << "\n";
        registerFile->print_registers();
        pc++;
    } catch (const std::out_of_range& e) {
        std::string exception = e.what();
        // distinguish between individual exceptions
        if (exception.find("vector::_M_range_check") != std::string::npos) {
            // out of range of raw_insts
            if (pc == raw_insts.size() + 1) {
                // one further than the size => EOF
                //TODO: implement termination function
                return EXEC_EOF;
            } else {
                //wrong address (pc)
                std::cerr << "Wrong instruction address: pc=" << pc*4 << "\n";
                //TODO: implement termination with error function
                return EXEC_ERROR;
            }
        } else if (exception.find("map::at") != std::string::npos) {
            // wrong opcode
            std::cerr << "Wrong opcode or not implemented instruction: opcode=" <<
                            std::bitset<7>(opcode).to_string() << "\n";
            //TODO: implement termination with error function
            return EXEC_ERROR;
        } else {
            // other error
            std::cerr << "Unknown error occurred\n";
            //TODO: implement termination with error function
            return EXEC_ERROR;
        }
    }
    return EXEC_OK;
}
