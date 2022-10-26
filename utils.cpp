#include "utils.h"

fplll::ZZ_mat<mpz_t> std_vector_to_fplll_ZZ_mat(std::vector<std::vector<int>> matrix) {
    if (matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = matrix.size();
    int cols = matrix.at(0).size();
    fplll::ZZ_mat<mpz_t> fplll_matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fplll_matrix(i, j) = matrix.at(i).at(j);
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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            eigen_matrix(i, j) = fplll_matrix(i, j).get_si();
        }
    }
    return eigen_matrix;
}


MatrixXm std_vector_to_Eigen_mat(std::vector<std::vector<int>> matrix) {
    if (matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int cols = matrix.size();
    int rows = matrix.at(0).size();
    MatrixXm eigen_matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            eigen_matrix(i, j) = matrix.at(j).at(i);
        }
    }
    return eigen_matrix;
}

fplll::ZZ_mat<mpz_t> eigen_mat_to_fplll_ZZ_mat(MatrixXm matrix) {
    if (matrix.size() <= 0) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    // These are flipped because we want to take the transpose
    int cols = matrix.cols();
    int rows = matrix.rows();
    fplll::ZZ_mat<mpz_t> fplll_matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mpz_t val;
            mpz_init(val);
            mpfr_get_z(val, matrix(i,j).mpfr_srcptr(), MPFR_RNDN);
            fplll_matrix(i,j) = val;
        }
    }
    return fplll_matrix;
}

MatrixXm read_matrix_from_file(std::string file, int rows, int cols) {
    MatrixXm matrix(rows, cols);
    std::fstream f(file);
    if (!f.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::string line;
    for (int i = 0; i < rows; i++) {
        if (!getline(f, line)) {
            throw std::runtime_error("Not enough rows in matrix file");
        }
        std::istringstream sstream(line);
        mpfr::mpreal val;

        for (int j = 0; j < cols; j++) {
            if (!(sstream >> val)) {
                throw std::runtime_error("Not enough columns in matrix file");
            }
            matrix(i, j) = val;
        }
    }
    return matrix;
}
