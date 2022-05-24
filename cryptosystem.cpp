#include "cryptosystem.h"

/**
 * Check if an n*n integer matrix generates Z^n.
 *
 * @param matrix A square integer matrix. Any all-zero rows must be on top.
 * @return True if matrix generates integer lattice, false otherwise.
 */
bool matrix_generates_integer_lattice(EigenMatrixXm matrix) {
    if (matrix.size() == 0) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    // If the determinant is +/- 1, matrix generates Z^n.
    return 0.5 < abs(matrix.determinant()) < 1.5;
}

void Cryptosystem::generate_keys() {
    EigenMatrixXm basis;
    mpfr::mpreal::set_default_prec(256);
    do {
        std::vector<std::vector<int>> sampled_matrix = sample_discrete_gaussian_vectors(k, n, s);
        fplll::ZZ_mat<mpz_t> sampled_matrix_fplll = std_vector_to_fplll_ZZ_mat(sampled_matrix);

        fplll::lll_reduction(sampled_matrix_fplll, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
                             fplll::LM_WRAPPER, fplll::FT_DEFAULT, 0, fplll::LLL_EARLY_RED);

        EigenMatrixXm sampled_matrix_eigen = fplll_ZZ_mat_to_Eigen_mat(sampled_matrix_fplll);
        arma::Mat<long> sampled_matrix_arma = fplll_ZZ_mat_to_arma_mat(sampled_matrix_fplll);
        // get the last n rows of sampled matrix
        basis = sampled_matrix_eigen.block(k-n, 0, n, n);

    } while (!matrix_generates_integer_lattice(basis));

    // It's more numerically stable to compute G^{-1} as B^{-1} * (B^{-1})^T than directly invert G
    arma::Mat<long> G_inv_arma = eigen_mat_to_arma_mat(basis.inverse() * basis.transpose().inverse());
    arma::Mat<long> G_arma = eigen_mat_to_arma_mat(basis.transpose() * basis);

    // Assert that the computed inverse is correct
    arma::Mat<long> identity(n, n, arma::fill::eye);
    assert(arma::approx_equal(G_arma * G_inv_arma, identity, "absdiff", EPS));
    assert(arma::approx_equal(G_inv_arma * G_arma, identity, "absdiff", EPS));

    B = eigen_mat_to_arma_mat(basis);
    G_inv = G_inv_arma;
}

arma::vec Cryptosystem::encrypt(arma::Mat<long> pk, bool b) {
    if (b) {
        return arma::randu<arma::vec>(n);
    }
    else {
        arma::mat covariance_mat = pow(r,2) / (2*M_PI) * arma::conv_to<arma::mat>::from(pk);

        // Check that there's no overflow issues when converting public key elements from longs to doubles.
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                assert(abs(covariance_mat(i,j) - pk(i,j)*pow(r,2)/(2*M_PI)) < 0.01);
            }
        }

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

bool Cryptosystem::decrypt(arma::Mat<long> sk, arma::vec c) {
    arma::vec t = sk * c;
    double dist_from_lattice = 0;
    for (int i=0; i<n; i++) {
        dist_from_lattice += pow(t(i) - round(t(i)), 2);
    }
    return dist_from_lattice > d;
}