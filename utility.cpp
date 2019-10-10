//
// Created by matej on 10/10/19.
//

#include "utility.h"

std::vector<int> utility::loadFile(const char *filename) {
    std::ifstream file;
    file.open(filename);
    std::string lines;

    //TODO: handle if the file doesn't exist!!
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
