/**
 * @file
 * @author Ryan Little
 */

#include <fplll.h>
#include <armadillo>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

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

/**
 * Reads an integer matrix in from a file. File must be space delimited and contain no additional formatting. Lines in
 * the file will correspond to rows in the matrix.
 *
 * @param file Path to the file.
 * @param rows Number of rows in matrix
 * @param cols Number of columns in matrix
 * @return The matrix from the file, converted to armadillo form.
 */
arma::Mat<int> read_from_file(std::string file, int rows, int cols);

#endif //ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
