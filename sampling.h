/**
 * @file
 * @author Ryan Little
 */

#ifndef ZNROTATIONCRYPTOSYSTEM_SAMPLING_H
#define ZNROTATIONCRYPTOSYSTEM_SAMPLING_H

#define CUTOFF_MODIFIER   10

#include <vector>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>
#include "cryptoTools/Crypto/PRNG.h"

/**
 * Samples a vector sampled from the one-dimensional discrete Gaussian D_{Z^n,s}. This is an implementation of the
 * SampleZ subroutine in section 4.1 of https://ia.cr/2007/432.
 *
 * @param n Dimension of vector to generate
 * @param s Parameter affecting length of sampled vector
 * @return An integer vector sampled from a discrete Gaussian distribution
 */
std::vector<int> sample_discrete_gaussian_vector(int n, double s);

/**
 * Samples multiple vectors from the one-dimensional discrete Gaussian D_{Z^n,s}.
 *
 * @param k Number of vectors to sample
 * @param n Dimension of vectors to be generated
 * @param s Parameter affecting length of sampled vector
 * @return A vector of k integer vectors, each independently sampled from a discrete Gaussian distribution
 */
std::vector<std::vector<int>> sample_discrete_gaussian_vectors(int k, int n, double s);


#endif //ZNROTATIONCRYPTOSYSTEM_SAMPLING_H
