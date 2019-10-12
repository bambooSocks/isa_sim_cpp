cmake_minimum_required(VERSION 3.15)
project(isa_sim_cpp)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        registerfile.cpp
        utility.cpp
)
set(HEADERS
        registerfile.h
        utility.h
)

add_executable(isa_sim_cpp ${SOURCES} ${HEADERS})