/**
 * @file
 * @author Ryan Little
 */

#include <Eigen/Dense>
#include <eigen3/unsupported/Eigen/MPRealSupport>
#include <fplll.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#ifndef ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
#define ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H

typedef Eigen::Matrix<mpfr::mpreal, Eigen::Dynamic, Eigen::Dynamic> MatrixXm;
typedef Eigen::Vector<mpfr::mpreal, Eigen::Dynamic> VectorXm;

/**
 * Converts a matrix from std::vector form to fplll form.
 *
 * @param matrix A std::vector of std::vectors.
 * @return The matrix in fplll format.
 */
fplll::ZZ_mat<mpz_t> std_vector_to_fplll_ZZ_mat(std::vector<std::vector<int>> matrix);

/**
 * Converts a matrix from fplll form to Eigen form (with mpfr type)
 * @param fplll_matrix A fplll integer matrix
 * @return The matrix in Eigen form
 */
MatrixXm fplll_ZZ_mat_to_Eigen_mat(fplll::ZZ_mat<mpz_t> fplll_matrix);

/**
 * Reads an integer matrix in from a file. File must be space delimited and contain no additional formatting. Lines in
 * the file will correspond to rows in the matrix.
 *
 * @param file Path to the file.
 * @param rows Number of rows in matrix
 * @param cols Number of columns in matrix
 * @return An Eigen matrix with mpfr entries containing the matrix from the file.
 */
MatrixXm read_matrix_from_file(std::string file, int rows, int cols);

#endif //ZNROTATIONCRYPTOSYSTEM_FPLLL_UTILS_H
