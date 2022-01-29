#include <fplll.h>
#include <armadillo>
#include <vector>

#ifndef ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
#define ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H


fplll::ZZ_mat<mpz_t> std_vector_to_fplll_ZZ_mat(std::vector<std::vector<int>> matrix);

arma::Mat<int> fplll_ZZ_mat_to_arma_mat(fplll::ZZ_mat<mpz_t> fplll_matrix);

#endif //ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
