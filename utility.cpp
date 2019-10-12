//
// Created by matej on 10/10/19.
//

#include "utility.h"

/**
 * Function for loading the binary file and converting it into
 * a standard vector of integers
 * @param filepath  the path to the binary file
 * @return          instructions in int vector
 */
std::vector<int> utility::loadFile(const char *filepath) {
    std::ifstream file;
    //TODO: handle if the file doesn't exist!!
    file.open(filepath);
    std::string lines;

    if (file.is_open()) {
        std::string line;
        while (getline (file, line)) {
            lines.append(line);
        }
        file.close();
    }
    int *temp = reinterpret_cast<int*>(lines.data());
    std::vector<int> inst;
    inst.insert(inst.end(), &temp[0], &temp[lines.length() / 4]);

    return inst;
}
