#include "fplll_utils.h"

fplll::ZZ_mat<mpz_t> std_vector_to_fplll_ZZ_mat(std::vector<std::vector<int>> matrix) {
    if (matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = matrix.size();
    int cols = matrix.at(0).size();
    fplll::ZZ_mat<mpz_t> fplll_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            fplll_matrix(i,j) = matrix.at(i).at(j);
        }
    }
    return fplll_matrix;
}

arma::Mat<int> fplll_ZZ_mat_to_arma_mat(fplll::ZZ_mat<mpz_t> fplll_matrix) {
    if (fplll_matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = fplll_matrix.r;
    int cols = fplll_matrix.c;
    arma::Mat<int> arma_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            // We're casting from an mpz_t to a long to an int here, so may get undefined behavior. But if this
            // function is only used with LLL reduced matrices that were initially sampled by (int) discrete gaussians
            // vectors, we may be okay?
            arma_matrix(i,j) = (int) fplll_matrix(i,j).get_si();
        }
    }
    return arma_matrix;
}