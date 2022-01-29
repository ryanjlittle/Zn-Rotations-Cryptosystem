cmake_minimum_required(VERSION 3.0)
project(ZnRotationCryptosystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -larmadillo -lfplll -lmpfr -lgmp")

include_directories(/usr/include/fplll)
add_executable(ZnRotationCryptosystem main.cpp sampling.cpp sampling.h utils.cpp utils.h cryptosystem.cpp cryptosystem.h)
