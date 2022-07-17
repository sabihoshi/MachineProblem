cmake_minimum_required(VERSION 3.22)
project(MachineProblem)

set(CMAKE_CXX_STANDARD 17)
file(GLOB source
        *.cpp
        *.h
        *.hpp
        )

add_executable(MachineProblem ${source})
