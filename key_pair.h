#ifndef ZNROTATIONCRYPTOSYSTEM_KEY_PAIR_H
#define ZNROTATIONCRYPTOSYSTEM_KEY_PAIR_H

#include "sampling.h"
#include "fplll_utils.h"
#include <armadillo>

class KeyPair {
private:
    std::vector<std::vector<int>> sampled_matrix;
    arma::Mat<int> B;
    arma::Mat<int> G;
    const int n;
    const int d;
    const int c;
    const double s;
    const double t;

public:
    KeyPair(int n, int d, int c, double s, double t) : n(n), d(d), c(c), s(s), t(t)  {}

    void generate_keys();

    const arma::Mat<int> &getB() { return B; }

    const arma::Mat<int> &getG() { return G; }
};
#endif //ZNROTATIONCRYPTOSYSTEM_KEY_PAIR_H
