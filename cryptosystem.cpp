#include "cryptosystem.h"

MatrixXm convert_to_basis(MatrixXm gen_set) {

    int m = gen_set.rows();
    int N = gen_set.cols();

    bool swap_condition;
    do {
        MatrixXm basis;
        Eigen::HouseholderQR<MatrixXm> QR(gen_set);
        MatrixXm R = QR.matrixQR().triangularView<Eigen::Upper>();

        std::vector<int> non_zero_gram_schmidt_indices;
        non_zero_gram_schmidt_indices.push_back(0);
        int k = 1;
        for (int i = 1; i < N; i++) {
            int j = k - 1;
            if (k < m && abs(R(k, i)) > EPS) {
                k++;
                non_zero_gram_schmidt_indices.push_back(i);
            }

            for (; j >= 0; j--) {
                int col = non_zero_gram_schmidt_indices.at(j);
                gen_set.col(i) -= round(R(j, i) / R(j, col)) * gen_set.col(col);
                R.col(i) -= round(R(j, i) / R(j, col)) * R.col(col);
            }
        }

        // Filter out identically zero columns from gen_set, delete corresponding columns from R as well.
        for (int i = N - 1; i >= 0; i--) {
            if (gen_set.col(i).isZero(EPS)) {
                N--;
                gen_set.block(0, i, m, N - i) = gen_set.rightCols(N - i);
                gen_set.conservativeResize(m, N);
                R.block(0, i, m, N - i) = R.rightCols(N - i);
                R.conservativeResize(m, N);
            }
        }

        swap_condition = false;
        // Find first zero Gram-Schmidt vector by finding first zero on R's diagonal
        int first_zero_gram_schmidt_index;
        for (int i = 1; i < m; i++) {
            if (abs(R(i, i)) < EPS) {
                swap_condition = true;
                first_zero_gram_schmidt_index = i;
                break;
            }
        }
        // If no zero found on R's diagonal, m-th Gram-Schmidt vector is the first zero G-S vector (if it exists)
        if (!swap_condition && m < N) {
            swap_condition = true;
            first_zero_gram_schmidt_index = m;
        }

        if (swap_condition) {
            // Find minimum index j such that the first zero gram-schmidt index is in span{b_1, ..., b_j}
            int j = 0;
            for (int i = first_zero_gram_schmidt_index - 1; i >= 0; i--) {
                if (abs(R(i, first_zero_gram_schmidt_index)) > EPS) {
                    j = i;
                    break;
                }
            }
            // Swap columns j and first_zero_gram_schmidt_index
            MatrixXm temp = gen_set.col(first_zero_gram_schmidt_index);
            gen_set.col(first_zero_gram_schmidt_index) = gen_set.col(j);
            gen_set.col(j) = temp;
        }
    } while (swap_condition);
    return gen_set;
}


void Cryptosystem::generate_keys() {
    MatrixXm basis;
    do {
        std::vector<std::vector<int>> gen_set = sample_discrete_gaussian_vectors(k, n, s);

        // Uncomment this and comment out the next 5 lines to run the alternate rotation-invariant basis reduction algo
        //  MatrixXm gen_set_eigen = std_vector_to_Eigen_mat(gen_set);
        //  basis = convert_to_basis(gen_set_eigen);

        fplll::ZZ_mat<mpz_t> sampled_matrix_fplll = std_vector_to_fplll_ZZ_mat(gen_set);
        fplll::lll_reduction(sampled_matrix_fplll, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
            fplll::LM_WRAPPER, fplll::FT_DEFAULT, 0, fplll::LLL_VERBOSE);
        MatrixXm sampled_matrix_eigen = fplll_ZZ_mat_to_Eigen_mat(sampled_matrix_fplll);
        // get the last n rows of sampled matrix
        basis = sampled_matrix_eigen.block(0, k-n, n, n);

        // Loop until 0.5 < det(basis) < 1.5, to ensure basis actually generates Z^n
    } while (!(0.5 < abs(basis.determinant()) && abs(basis.determinant()) < 1.5));

    MatrixXm G = basis.transpose() * basis;
    // It's more numerically stable to compute G^{-1} as B^{-1} * (B^{-1})^T than directly invert G
    MatrixXm B_inv = basis.inverse();
    MatrixXm G_inv = B_inv * B_inv.transpose();

    // Assert that the computed inverse is correct
    MatrixXm identity = MatrixXm::Identity(n, n);
    assert((G_inv * G).isApprox(identity, EPS));
    assert((G * G_inv).isApprox(identity, EPS));

    this->B = basis;
    this->G_inv = G_inv;
}

MatrixXm Cryptosystem::encrypt_rep_code(MatrixXm pk, bool b, int m) {
    assert(m%2 == 1);
    MatrixXm ctext(this->n, m);
    for (int i = 0; i < m; i++) {
        ctext.col(i) = this->encrypt(pk, b);
    }
    return ctext;
}

bool Cryptosystem::decrypt_rep_code(MatrixXm sk, MatrixXm c, int m) {
    assert(m%2 == 1);
    int ones = 0;
    for (int i = 0; i < m; i++) {
        if (this->decrypt(sk, c.col(i))) {
            ones++;
        }
        // Exit early if we have a majority of 1s or 0s
        if (ones > m / 2) {
            return 1;
        } else if (i - ones > m / 2) {
            return 0;
        }
    }
    return ones > m / 2; // Unreachable so long as m is odd
}

VectorXm Cryptosystem::encrypt(MatrixXm pk, bool b) {
    if (b) {
        std::uniform_real_distribution<double> uniform_distr; // uniform distribution over [0,1)
        osuCrypto::PRNG prng;
        prng.SetSeed(osuCrypto::sysRandomSeed());
        VectorXm sampled_vec = VectorXm(n);
        for (int i = 0; i < n; i++) {
            sampled_vec(i) = uniform_distr(prng);
        }
        return sampled_vec;
    } else {

        VectorXm zeros = VectorXm::Zero(n);
        MatrixXm covariance_mat = pow(r, 2) / (2 * M_PI) * pk;

        Eigen::EigenMultivariateNormal<mpfr::mpreal> sampler(zeros, covariance_mat);
        MatrixXm sampled_vec = sampler.samples(1);

        // return sampled vector modulo 1
        for (int i = 0; i < n; i++) {
            mpfr::mpreal elem_mod_1 = fmod(sampled_vec(i), 1);
            if (elem_mod_1 < 0) {
                elem_mod_1 += 1;
            }
            sampled_vec(i) = elem_mod_1;
        }
        return sampled_vec;
    }
}

bool Cryptosystem::decrypt(MatrixXm sk, VectorXm c) {

    MatrixXm t = sk * c;
    mpfr::mpreal dist_from_lattice = 0;
    for (int i = 0; i < n; i++) {
        dist_from_lattice += pow(t(i) - round(t(i)), 2);
    }
    return dist_from_lattice > d;
}

