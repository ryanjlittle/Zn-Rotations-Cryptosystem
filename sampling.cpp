#include "sampling.h"

// Returns a vector sampled from the one-dimensional discrete Gaussian D_{Z,s}. This is an implementation of the
// SampleZ subroutine in section 4.1 of https://ia.cr/2007/432
std::vector<int> sample_discrete_gaussian_vector(int n, double c, double s, double t) {
    std::random_device generator;
    std::uniform_int_distribution<int> uniform_dist(ceil(c-t*s), floor(c+t*s));
    std::bernoulli_distribution bernoulli_dist;

    std::vector<int> sampled_vector(n);
    int x, p;
    for (int i=0; i<n; i++) {
        do {
            x = uniform_dist(generator);
            bernoulli_dist = std::bernoulli_distribution(exp(-M_PI*pow(x,2) / pow(s, 2)));
            p = bernoulli_dist(generator);
        }
        while (p == 0);
        sampled_vector.at(i) = x;
    }
    return sampled_vector;
}

// TODO: FIX THIS! Does not actually output generating sets!
std::vector<std::vector<int>> sample_Zn_generating_set(int n, double c, double s, double t) {
    std::vector<std::vector<int>> generating_set;
    std::vector<int> column_GCDs(n, 0);

    while(std::any_of(column_GCDs.begin(), column_GCDs.end(), [](int k) {return k!=1;})) {
        std::vector<int> vec = sample_discrete_gaussian_vector(n, c, s, t);
        for (int i=0; i<n; i++) {
            if (column_GCDs.at(i) != 1) {
                column_GCDs.at(i) = std::gcd(column_GCDs.at(i), vec.at(i));
            }
        }
        generating_set.push_back(vec);
    }
    return generating_set;
}

