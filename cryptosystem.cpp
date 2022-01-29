#include "cryptosystem.h"

void Cryptosystem::generate_keys() {
    arma::Mat<int> basis;
    double log_det_val, log_det_sign;
    do {
        std::vector<std::vector<int>> sampled_matrix = sample_discrete_gaussian_vectors(k, n, c, s, t);
        fplll::ZZ_mat<mpz_t> sampled_matrix_fplll = std_vector_to_fplll_ZZ_mat(sampled_matrix);

        fplll::lll_reduction(sampled_matrix_fplll, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
                             fplll::LM_FAST, fplll::FT_DEFAULT, 0, fplll::LLL_DEFAULT);

        arma::Mat<int> sampled_matrix_arma = fplll_ZZ_mat_to_arma_mat(sampled_matrix_fplll);
        basis = sampled_matrix_arma.rows(k-n, k-1); // get the last n rows
        arma::log_det(log_det_val, log_det_sign, arma::conv_to<arma::mat>::from(basis)); // log det more precise than det for large matrices
    } while (abs(log_det_val) > EPS);

    B = basis;
    G = B.t() * B;
}

arma::vec Cryptosystem::encrypt(arma::Mat<int> pk, bool b) {
    if (b) {
        return arma::randu<arma::vec>(n);
    }
    else {
        // inv_sympd computes inverses for symmetric positive definite matrices (and is faster than inverting general matrices). G is always symmetric positive definite... right?
        arma::mat covariance_mat = pow(r,2) * arma::inv_sympd(arma::conv_to<arma::mat>::from(pk));
        arma::vec zeros(n, arma::fill::zeros);
        arma::vec sampled_vec = arma::mvnrnd(zeros, covariance_mat);
        // return sampled vector modulo 1
        for (int i=0; i<n; i++) {
            double elem_mod_1 = fmod(sampled_vec(i), 1);
            if (elem_mod_1 < 0) {
                elem_mod_1 += 1;
            }
            sampled_vec(i) = elem_mod_1;
        }
        return sampled_vec;
    }
}

bool Cryptosystem::decrypt(arma::Mat<int> sk, arma::vec ctext) {
    arma::vec t = sk * ctext;
    double dist_from_lattice = 0;
    for (int i=0; i<n; i++) {
        dist_from_lattice += pow(t(i) - round(t(i)), 2);
    }
    return dist_from_lattice > d;
}