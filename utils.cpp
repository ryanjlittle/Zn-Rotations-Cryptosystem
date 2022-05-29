#include "utils.h"

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

MatrixXm fplll_ZZ_mat_to_Eigen_mat(fplll::ZZ_mat<mpz_t> fplll_matrix) {
    if (fplll_matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = fplll_matrix.r;
    int cols = fplll_matrix.c;
    MatrixXm eigen_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            eigen_matrix(i,j) = fplll_matrix(i,j).get_si();
        }
    }
    return eigen_matrix;
}

MatrixXm read_matrix_from_file(std::string file, int rows, int cols){
    MatrixXm matrix(rows, cols);
    std::fstream f(file);
    if (!f.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::string line;
    for (int i=0; i<rows; i++) {
        if (!getline(f, line)) {
            throw std::runtime_error("Not enough rows in matrix file");
        }
        std::istringstream sstream(line);
        mpfr::mpreal val;

        for (int j=0; j<cols; j++) {
            if (!(sstream >> val)) {
                throw std::runtime_error("Not enough columns in matrix file");
            }
            matrix(i,j) = val;
        }
    }
    return matrix;
}
