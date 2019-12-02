# register_file.cpp
# Matej Majtan (s184457) & Søren Tønnesen (s180381)
# 02-12-2019

cmake_minimum_required(VERSION 3.13)
set(EXECUTABLE isa_sim_cpp)

project(${EXECUTABLE})

set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17)

# uncomment to enable debug messages
#add_definitions(-DDEBUG)

set(SOURCES
        main.cpp
        isa_simulator.cpp
        register_file.cpp
        instruction_decoder.cpp
        stack.cpp
        termination.cpp)

set(HEADERS
        isa_simulator.h
        register_file.h
        instruction_decoder.h
        stack.h
        termination.h)

add_executable(${EXECUTABLE} ${SOURCES} ${HEADERS})

target_link_libraries(${EXECUTABLE} stdc++fs)
