#include "key_pair.h"

void KeyPair::generate_keys() {
    arma::Mat<int> basis;
    double determinant;
    do {
        sampled_matrix = sample_discrete_gaussian_vectors(d, n, c, s, t);

        fplll::ZZ_mat<mpz_t> sampled_matrix_fplll = std_vector_to_fplll_ZZ_mat(sampled_matrix);
        fplll::lll_reduction(sampled_matrix_fplll, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
                             fplll::LM_FAST, fplll::FT_DEFAULT, 0, fplll::LLL_DEFAULT);
        sampled_matrix_fplll.print_comma(std::cout);

        arma::Mat<int> sampled_matrix_arma = fplll_ZZ_mat_to_arma_mat(sampled_matrix_fplll);
        basis = sampled_matrix_arma.rows(n, d-1);
        determinant = arma::det(arma::conv_to<arma::mat>::from(basis));
    } while (abs(determinant) != 1 );

    B = basis;
    G = B.t() * B;
}


