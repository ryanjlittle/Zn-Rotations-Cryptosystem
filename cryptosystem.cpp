#include "cryptosystem.h"

void Cryptosystem::generate_keys() {
    MatrixXm basis;
    do {
        std::vector<std::vector<int>> sampled_matrix = sample_discrete_gaussian_vectors(k, n, s);
        fplll::ZZ_mat<mpz_t> sampled_matrix_fplll = std_vector_to_fplll_ZZ_mat(sampled_matrix);

        fplll::lll_reduction(sampled_matrix_fplll, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
                             fplll::LM_WRAPPER, fplll::FT_DEFAULT, 0, fplll::LLL_EARLY_RED);

        MatrixXm sampled_matrix_eigen = fplll_ZZ_mat_to_Eigen_mat(sampled_matrix_fplll);

        // get the last n rows of sampled matrix
        basis = sampled_matrix_eigen.block(k-n, 0, n, n);

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


VectorXm Cryptosystem::encrypt(MatrixXm pk, bool b) {
    if (b) {
        std::uniform_real_distribution<double> uniform_distr; // uniform distribution over [0,1)
        osuCrypto::PRNG prng;
        prng.SetSeed(osuCrypto::sysRandomSeed());
        VectorXm sampled_vec = VectorXm(n);
        for (int i=0; i<n; i++) {
            sampled_vec(i) = uniform_distr(prng);
        }
        return sampled_vec;
    }
    else {

        VectorXm zeros = VectorXm::Zero(n);
        MatrixXm covariance_mat = pow(r, 2) / (2*M_PI) * pk;

        Eigen::EigenMultivariateNormal<mpfr::mpreal> sampler(zeros, covariance_mat);
        MatrixXm sampled_vec = sampler.samples(1);

        // return sampled vector modulo 1
        for (int i=0; i<n; i++) {
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
    for (int i=0; i<n; i++) {
        dist_from_lattice += pow(t(i) - round(t(i)), 2);
    }
    return dist_from_lattice > d;
}