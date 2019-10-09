#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main () {
    std::ifstream file;
    file.open("../test/main");
    std::string lines;

    if (file.is_open()) {
        std::string line;
        while ( getline (file, line)) {
            lines.append(line);
        }
        file.close();
    }
    int *temp = reinterpret_cast<int*>(lines.data());

    std::vector<int> inst;
    inst.insert(inst.end(), &temp[0], &temp[lines.length() / 4]);

    std::cout << inst.at(0) << std::endl;
    return 0;
}
