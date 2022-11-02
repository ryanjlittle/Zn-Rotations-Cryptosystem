#include <iostream>
#include "cryptosystem.h"
#include "generateChallenges.h"
#include "LLLattack.h"

double decryption_correctness_prob_uniform(Cryptosystem cryptosystem, int num_trials) {
    int correct_decryptions = 0;
    for (int i = 0; i < num_trials; i++) {
//        VectorXm ctext = cryptosystem.encrypt(cryptosystem.get_G_inv(), true);
//        bool decryption = cryptosystem.decrypt(cryptosystem.get_B(), ctext);
        MatrixXm ctext = cryptosystem.encrypt_rep_code(cryptosystem.get_G_inv(), true, 3);
        bool decryption = cryptosystem.decrypt_rep_code(cryptosystem.get_B(), ctext, 3);
        if (decryption) {
            correct_decryptions++;
        }
    }
    return double(correct_decryptions) / double(num_trials);
}

double decryption_correctness_prob_near_lattice(Cryptosystem cryptosystem, int num_trials) {
    int correct_decryptions = 0;
    for (int i = 0; i < num_trials; i++) {
//        VectorXm ctext = cryptosystem.encrypt(cryptosystem.get_G_inv(), false);
//        bool decryption = cryptosystem.decrypt(cryptosystem.get_B(), ctext);
        MatrixXm ctext = cryptosystem.encrypt_rep_code(cryptosystem.get_G_inv(), false, 3);
        bool decryption = cryptosystem.decrypt_rep_code(cryptosystem.get_B(), ctext, 3);
        if (!decryption) {
            correct_decryptions++;
        }
    }
    return double(correct_decryptions) / double(num_trials);
}

void quick_test() {
    int n = 128;
    double r = 0.081583;
    double d = 0.0421963 * n;
    int s = 10;
    int k = n + 10;
    int num_trials = 10;

    Cryptosystem cryptosystem(n, k, r, s, d);
    std::cout << "Generating keys..." << std::endl;
    cryptosystem.generate_keys();
    std::cout << "Keys generated." << std::endl;

    double correct_prob_uniform = decryption_correctness_prob_uniform(cryptosystem, num_trials);
    double correct_prob_near_lattice = decryption_correctness_prob_near_lattice(cryptosystem, num_trials);
    std::cout << std::fixed << correct_prob_uniform << " correctness probability, uniform sample" << std::endl;
    std::cout << std::fixed << correct_prob_near_lattice << " correctness probability, near-lattice sample"
              << std::endl;
}


void makeChallenges() {
    // Modify these values for each challenge
    int n = 512;
    double r = 0.081583;
    double d = 0.0421963 * n;
    double s = 100;
    Cryptosystem cryptosystem(n, n + 10, r, s, d);
    generateChallenges(cryptosystem, 20, "challenges/n512_2E-40");
}

void testAttack() {
    int n = 128;
    double rs[] = {0.00081583, 0.0081583, 0.081583, 0.81583, 8.1583};
    double bk_sizes[] = {2, 3, 5, 10};
    Cryptosystem cryptosystem(n, n + 10, 0, 10, 0);
    cryptosystem.generate_keys();
    MatrixXm B = cryptosystem.get_B();
    MatrixXm G_inv = cryptosystem.get_G_inv();

    int num_trials = 20;
    for (int i=3; i<5; i++) {
        std::cout << "=========== r = " << rs[i] << " ==============\n";
        cryptosystem = Cryptosystem(n, n+10, rs[i], 10, 0.0421963 * 128);
        cryptosystem.set_B(B);
        cryptosystem.set_G_inv(G_inv);
        LLLattack lllattack(cryptosystem);
        for (int j=0; j<num_trials; j++) {
            VectorXm c = cryptosystem.encrypt(cryptosystem.get_G_inv(), false);
            int k=0;
            int block_size;
            do {
                block_size = bk_sizes[k];
                k++;
                if (!lllattack.decrypt(c, block_size)) {
                    std::cout << "Decrypted successfully with block size = " << block_size << std::endl;
                    break;
                }
                if (k==4) {
                    std::cout << "Not decrypted\n";
                }
            } while(k < 4);
        }
    }
}

void makeChallengesRepCode() {

    int ns[] = {128, 256, 256, 512, 512, 512, 512};
    double rs[] = {0.081583, 0.39872, 0.081583, 0.530363, 0.39872, 0.275526, 0.081583};
    double ds[] = {0.0421963, 0.0542451, 0.0421963, 0.0627648, 0.0542451, 0.0477076, 0.0421963};
    std::string challenge_names[] = {"n128_fp2-10", "n256_fp2-10", "n256_fp2-20", "n512_fp2-10", "n512_fp2-20", "n512_fp2-30", "n512_fp2-40"};
    double s = 10;
    int reps = 3;
    int num_challenges = 20;
    for (int i=2; i<7; i++) {
        std::string sk_file = "../keys/" + challenge_names[i] + "/private_key";
        std::string pk_file = "../keys/" + challenge_names[i] + "/public_key";
        std::string out_path = "../challenges/" + challenge_names[i];
        Cryptosystem cryptosystem(ns[i], ns[i] + 10, rs[i], s, ds[i]*ns[i]);
        cryptosystem.set_B(read_matrix_from_file(sk_file, ns[i], ns[i]));
        cryptosystem.set_G_inv(read_matrix_from_file(pk_file, ns[i], ns[i]));

        generateChallengesRepCode(cryptosystem, num_challenges, reps, out_path);
    }
}


int main() {
    mpfr::mpreal::set_default_prec(256);
    // Uncomment one of the following to run a test

    quick_test();
//    makeChallenges();
//    testAttack();
//    makeChallenges();
//    makeChallengesRepCode();

    return 0;
}
