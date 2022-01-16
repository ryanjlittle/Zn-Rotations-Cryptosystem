cmake_minimum_required(VERSION 3.0)
project(ZnRotationCryptosystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lmpfr -lfplll -lgmp")

add_executable(ZnRotationCryptosystem main.cpp sampling.cpp sampling.h fplll_utils.cpp fplll_utils.h)
