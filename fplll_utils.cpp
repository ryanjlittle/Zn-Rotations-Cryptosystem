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