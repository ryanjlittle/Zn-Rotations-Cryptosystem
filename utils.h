/**
 * @file
 * @author Ryan Little
 */

#include <fplll.h>
#include <armadillo>
#include <vector>

#ifndef ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
#define ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H


/**
 * Converts a matrix from std::vector form to fplll form.
 *
 * @param matrix A std::vector of std::vectors.
 * @return The matrix in fplll format.
 */
fplll::ZZ_mat<mpz_t> std_vector_to_fplll_ZZ_mat(std::vector<std::vector<int>> matrix);

/**
 * Converts a matrix from fplll form to armadillo form.
 *
 * @param fplll_matrix A fplll integer matrix.
 * @return The matrix in armadillo form.
 */
arma::Mat<int> fplll_ZZ_mat_to_arma_mat(fplll::ZZ_mat<mpz_t> fplll_matrix);

#endif //ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
