cmake_minimum_required(VERSION 3.0)
project(ZnRotationCryptosystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -lpython3.10 -larmadillo -lfplll -lmpfr -lgmp")

add_executable(ZnRotationCryptosystem main.cpp cryptosystem.cpp sampling.cpp utils.cpp generateChallenges.cpp)

find_package(cryptoTools REQUIRED)
target_link_libraries(ZnRotationCryptosystem oc::cryptoTools)
