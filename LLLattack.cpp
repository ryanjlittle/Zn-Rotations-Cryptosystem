#include "LLLattack.h"

LLLattack::LLLattack(Cryptosystem cryptosystem) {
    this->G_inv = cryptosystem.get_G_inv();
    this->n = cryptosystem.get_n();
    this->d = cryptosystem.get_d();

    MatrixXm G = cryptosystem.get_B().transpose() * cryptosystem.get_B();
    // Get Cholesky decomposition of G
    Eigen::LLT<MatrixXm> cholesky(this->G_inv);
    this->L = cholesky.matrixL().toDenseMatrix().inverse();
//    std::cout << "L:\n" << L << std::endl;
    assert((L.transpose() * L).isApprox(G, 1e-10));
}

bool LLLattack::decrypt(VectorXm c) {
    MatrixXm B_prime(n, n+1);
    B_prime << this->L, this->L * c;
    fplll::ZZ_mat<mpz_t> B_prime_fplll = eigen_mat_to_fplll_ZZ_mat(MULT_CONST * B_prime);


//    std::cout << "B' before reducing: " << B_prime.rows() << "x" << B_prime.cols() << std::endl;
//    std::cout << B_prime << std::endl;

    fplll::lll_reduction(B_prime_fplll, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
            fplll::LM_PROVED, fplll::FT_MPFR, fplll::LLL_VERBOSE);

//    MatrixXm reduced = fplll_ZZ_mat_to_Eigen_mat(B_prime_fplll) / MULT_CONST;
//    std::cout << "\nB' after reducing: " << reduced.rows() << "x" << reduced.cols() << std::endl;
//    std::cout << reduced << std::endl;

    MatrixXm v = fplll_ZZ_mat_to_Eigen_mat(B_prime_fplll).col(0) / MULT_CONST;

//    std::cout << "\nv:\n" << v << std::endl;
//    std::cout << "\n||v||^2:\n" << v.squaredNorm() << std::endl;

    if (v.squaredNorm() > this->d ) {
        return true;
    }

    VectorXm xy = B_prime.colPivHouseholderQr().solve(v);
    assert((B_prime * xy).isApprox(v));

//    std::cout << "\nxy:\n" << xy << std::endl;

    return abs(abs(xy[n]) - 1) < EPS;

}
