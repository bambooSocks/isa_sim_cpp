//
// Created by matej on 10/14/19.
//

#include <filesystem>
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
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
        std::string line;
        while (std::getline (file, line)) {
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
    unsigned char opcode = 0;
    try {
        // fetch instruction
        unsigned int inst = raw_insts.at(pc / 4);
        // decode instruction and execute
        opcode = inst & 0x0000007Fu;

        // execute instruction and update pc
        pc = opcode_map.at(opcode)->decode(pc, inst);

        if (int(pc) == -1) {
            terminate("ECALL instruction reached");
        }

#ifdef DEBUG
        std::cout << "\nProgram counter: " << std::dec << pc << "\n";
        registerFile->print_registers();
#endif

    } catch (const std::out_of_range& e) {
        std::string exception = e.what();
        // distinguish between individual exceptions
        if (exception.find("vector::_M_range_check") != std::string::npos) {
            // out of range of raw_insts
            //TODO: test this
            if (pc == raw_insts.size()*4 + 4) {
                // one further than the size => EOF
                terminate("End of file reached");
                return EXEC_EOF;
            } else {
                //wrong address (pc)
                terminateWithError("Wrong instruction address: pc = " + std::to_string(pc), -1);
                return EXEC_ERROR;
            }
        } else if (exception.find("map::at") != std::string::npos) {
            // wrong opcode
            terminateWithError("Wrong opcode or not implemented instruction: opcode="
                               + std::bitset<7>(opcode).to_string(), -1);
            return EXEC_ERROR;
        } else {
            // other error
            terminateWithError("Unknown error occurred", -1);
            return EXEC_ERROR;
        }
    }
    return EXEC_OK;
}

void ISA_Simulator::terminate (const std::string& msg) {
    std::cout << "\x1B[32m" << msg << "\x1B[0m\r\n\r\n";
    registerFile->print_registers();
    exit(0);
}

void ISA_Simulator::terminateWithError (const std::string& msg, int exit_code) {
    std::cout << "\x1B[31m" << msg << "\x1B[0m\r\n";
    std::cout << "\x1B[31mTerminated with exit code: " << std::dec << int(exit_code) << "\x1B[0m\r\n\r\n";
    registerFile->print_registers();
    exit(exit_code);
}
