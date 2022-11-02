#include "LLLattack.h"

LLLattack::LLLattack(Cryptosystem cryptosystem) {
    this->G_inv = cryptosystem.get_G_inv();
    this->n = cryptosystem.get_n();
    this->d = cryptosystem.get_d();

    MatrixXm G = cryptosystem.get_B().transpose() * cryptosystem.get_B();
    // Get Cholesky decomposition of G
    Eigen::LLT<MatrixXm> cholesky(this->G_inv);
    this->L = cholesky.matrixL().toDenseMatrix().inverse();
    assert((L.transpose() * L).isApprox(G, 1e-10));
}

bool LLLattack::decrypt(VectorXm c) {
    MatrixXm B_prime_x(n, n+1);
    B_prime_x << this->L, this->L * c;

    MatrixXm orthog_comp(1, n+1);
    for (int i=0; i<n; i++) {
        orthog_comp(i) = 0;
    }
    orthog_comp(n) = 1e-5;

    MatrixXm B_prime(n+1, n+1);
    B_prime << B_prime_x, orthog_comp;

    // Run BKZ on B_prime. Since fplll requires integer valued inputs, we multiply by a large constant MULT_CONST and
    // round to nearest integer before BKZ reducing.
    fplll::ZZ_mat<mpz_t> B_prime_fplll = eigen_mat_to_fplll_ZZ_mat(MULT_CONST * B_prime);
    int block_size = 5;
    fplll::bkz_reduction(B_prime_fplll, block_size);
    MatrixXm reduced = fplll_ZZ_mat_to_Eigen_mat(B_prime_fplll) / MULT_CONST;

    MatrixXm v = fplll_ZZ_mat_to_Eigen_mat(B_prime_fplll).col(0) / MULT_CONST;
    if (v.squaredNorm() > this->d ) {
        return true;
    }

    VectorXm xy = B_prime.colPivHouseholderQr().solve(v);
    assert((B_prime * xy).isApprox(v));

    return abs(abs(xy[n]) - 1) > EPS;
}
