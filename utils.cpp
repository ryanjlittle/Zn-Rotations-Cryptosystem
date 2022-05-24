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

arma::Mat<long> fplll_ZZ_mat_to_arma_mat(fplll::ZZ_mat<mpz_t> fplll_matrix) {
    if (fplll_matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = fplll_matrix.r;
    int cols = fplll_matrix.c;
    arma::Mat<long> arma_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            arma_matrix(i,j) = (long) fplll_matrix(i,j).get_si();
        }
    }
    return arma_matrix;
}

EigenMatrixXm fplll_ZZ_mat_to_Eigen_mat(fplll::ZZ_mat<mpz_t> fplll_matrix) {
    if (fplll_matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = fplll_matrix.r;
    int cols = fplll_matrix.c;
    EigenMatrixXm eigen_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            eigen_matrix(i,j) = fplll_matrix(i,j).get_si();
        }
    }
    return eigen_matrix;
}

arma::Mat<long> eigen_mat_to_arma_mat(EigenMatrixXm eigen_matrix) {
    if (eigen_matrix.size() == 0) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = eigen_matrix.rows();
    int cols = eigen_matrix.cols();
    arma::Mat<long> arma_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            long rounded = mpfr_get_si(eigen_matrix(i,j).mpfr_srcptr(), MPFR_RNDN);
//            assert(abs(rounded - eigen_matrix(i,j)) < 0.01);
            arma_matrix(i,j) = rounded;
        }
    }
    return arma_matrix;
}


EigenMatrixXm arma_mat_to_eigen_mat(arma::Mat<long> arma_matrix) {
    if (arma_matrix.empty()) {
        throw std::invalid_argument("Provided matrix must be non-empty");
    }
    int rows = arma_matrix.n_rows;
    int cols = arma_matrix.n_cols;
    EigenMatrixXm eigen_matrix(rows, cols);
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++) {
            eigen_matrix(i,j) = arma_matrix(i,j);
        }
    }
    return eigen_matrix;
}

arma::Mat<long> read_from_file(std::string file, int rows, int cols){
   arma::Mat<long> matrix(rows, cols);
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
       long val;
       for (int j=0; j<cols; j++) {
          if (!(sstream >> val)) {
              throw std::runtime_error("Not enough columns in matrix file");
          }
          matrix(i,j) = val;
       }
   }
   return matrix;
}