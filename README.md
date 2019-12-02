# RISC-V ISA simulator

This project is final assignment for 02155 Computer Architecture and Engineering course at DTU.

### Dependencies

There are no external dependencies but in order to compile the software `cmake` and `g++` must be installed. The minimum version of `g++` must be 8 or newer. The minimum version of `cmake` must be 3.13 or newer.

### Compilation

In order to build the software run these commands in the root directory of the project:

```bash
mkdir build
cd build
cmake ..
make
```

### Running the program

In order to run the software run the executable in `build` folder using command: `./isa_sim_cpp <path_to_binary>`. The `<path_to_binary>` denotes the path to the binary file.