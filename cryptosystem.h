#ifndef ZNROTATIONCRYPTOSYSTEM_CRYPTOSYSTEM_H
#define ZNROTATIONCRYPTOSYSTEM_CRYPTOSYSTEM_H

#define EPS 1E-12

#include "sampling.h"
#include "utils.h"
#include <math.h>

class Cryptosystem {
private:
    arma::Mat<int> B; // private key
    arma::Mat<int> G; // public key
    const int n;
    const int k;
    const double c;
    const double r;
    const double s;
    const double d;
    const double t;

public:
    Cryptosystem(int n, int k, double c, double r, double s, double d, double t) : n(n), k(k), c(c), r(r), s(s), d(d), t(t)  {}

    void generate_keys();

    arma::vec encrypt(arma::Mat<int> pk, bool b);

    bool decrypt(arma::Mat<int> sk, arma::vec ctext);

    const arma::Mat<int> &getB() { return B; }

    const arma::Mat<int> &getG() { return G; }
};

#endif //ZNROTATIONCRYPTOSYSTEM_CRYPTOSYSTEM_H
