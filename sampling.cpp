#include "sampling.h"

// Returns a vector sampled from the one-dimensional discrete Gaussian D_{Z,s}. This is an implementation of the
// SampleZ subroutine in section 4.1 of https://ia.cr/2007/432
std::vector<int> sample_discrete_gaussian(int n, double c, double s, double t) {
    std::random_device generator;
    std::uniform_int_distribution<int> uniform_dist(ceil(c-t*s), floor(c+t*s));
    std::bernoulli_distribution bernoulli_dist;

    std::vector<int> sampled(n);
    int x, p;
    for (int i=0; i<n; i++) {
        do {
            x = uniform_dist(generator);
            bernoulli_dist = std::bernoulli_distribution(exp(-M_PI*pow(x,2) / pow(s, 2)));
            p = bernoulli_dist(generator);
        }
        while (p == 0);
        sampled.at(i) = x;
    }
    return sampled;
}

// Samples vectors independently from the one-dimensional discrete Gaussian until they generate Z_n. To tell when the
// vectors generate Z_n, we store a vector for the GCD of the basis vectors in each element. The GCD vector is updated
// for each new sampled basis vector, and when the GCD is 1 for all n dimensions, the sampled vectors are a generating
// set of Z_n.
std::vector<std::vector<int>> sample_Zn_basis(int n, double c, double s, double t) {
    std::vector<std::vector<int>> basis;
    std::vector<int> column_GCDs(n, 0);

    while(std::any_of(column_GCDs.begin(), column_GCDs.end(), [](int k) {return k!=1;})) {
        std::vector<int> vec = sample_discrete_gaussian(n, c, s, t);
        for (int i=0; i<n; i++) {
            if (column_GCDs.at(i) != 1) {
                column_GCDs.at(i) = std::gcd(column_GCDs.at(i), vec.at(i));
            }
        }
        basis.push_back(vec);
    }
    return basis;
}

