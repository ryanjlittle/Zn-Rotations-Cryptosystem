#include "sampling.h"

std::vector<int> sample_discrete_gaussian_vector(int n, double s) {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> uniform_dist(ceil(-CUTOFF_MODIFIER*s), floor(CUTOFF_MODIFIER*s));
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

std::vector<std::vector<int>> sample_discrete_gaussian_vectors(int k, int n, double s) {
    std::vector<std::vector<int>> sampled_vectors(k);
    for (int i=0; i<k; i++) {
        sampled_vectors.at(i) = sample_discrete_gaussian_vector(n, s);
    }
    return sampled_vectors;
}
