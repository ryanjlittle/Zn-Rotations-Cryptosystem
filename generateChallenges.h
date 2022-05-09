#ifndef ZNROTATIONCRYPTOSYSTEM__GENERATECHALLENGES_H_
#define ZNROTATIONCRYPTOSYSTEM__GENERATECHALLENGES_H_

#include "cryptosystem.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>

/**
 * Generates challenges from a given cryptosystem. The public/private keys, ciphertexts, and plaintexts are all written
 * to files saved at the given path.
 *
 * @param cryptosystem The cryptosystem that will generate the challenges
 * @param num_challenges Number of ciphertext challenges to generate
 * @param path Path to store challenges and solutions
 */
void generateChallenges(Cryptosystem cryptosystem, int num_challenges, std::string path);

#endif //ZNROTATIONCRYPTOSYSTEM__GENERATECHALLENGES_H_
