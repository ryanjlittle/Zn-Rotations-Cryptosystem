#include <iostream>
#include "cryptosystem.h"
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

double decryption_correctness_prob_uniform(Cryptosystem cryptosystem, int num_trials) {
    int correct_decryptions = 0;
    for (int i = 0; i < num_trials; i++) {
        arma::vec ctext = cryptosystem.encrypt(cryptosystem.getG (), false);
        bool decryption = cryptosystem.decrypt(cryptosystem.getB (), ctext);
        if (!decryption) {
            correct_decryptions++;
        }
    }
    return double(correct_decryptions) / double(num_trials);
}

double decryption_correctness_prob_near_lattice(Cryptosystem cryptosystem, int num_trials) {
    int correct_decryptions = 0;
    for (int i = 0; i < num_trials; i++) {
        arma::vec ctext = cryptosystem.encrypt(cryptosystem.getG (), true);
        bool decryption = cryptosystem.decrypt(cryptosystem.getB (), ctext);
        if (decryption) {
            correct_decryptions++;
        }
    }
    return double(correct_decryptions) / double(num_trials);
}

void generate_plots() {
    int num_trials = 1000;
    int s = 10;
    std::vector<int> n_vals {32, 64, 128};
    std::vector<double> r_vals {0.1, 0.25, 0.4};
    std::vector<double> d_vals;
    std::vector<double> correctness_uniform, correctness_near_lattice;
    int plot_col;
    int plot_row = 0;

    arma::arma_rng::set_seed_random();
    std::cout.precision(3);

    for (int n : n_vals) {
        int k = n + 10;
        Cryptosystem cryptosystem(n, k, 0, s, 0);
        cryptosystem.generate_keys();
        plot_col = 0;

        for (double r : r_vals) {

            correctness_uniform.clear();
            correctness_near_lattice.clear();
            d_vals.clear();

            double d_prime = exp(-M_PI * r * r) / 20.0;
            double d_calculated = (1.0 / 12.0 - d_prime) * n;

            for (double d = 0.1; d < 12; d += 0.4) {
                d_vals.push_back (d);

                cryptosystem.set_r (r);
                cryptosystem.set_d (d);

                double correct_prob_uniform = decryption_correctness_prob_uniform(cryptosystem, num_trials);
                double correct_prob_near_lattice = decryption_correctness_prob_near_lattice(cryptosystem, num_trials);
                correctness_uniform.push_back(correct_prob_uniform);
                correctness_near_lattice.push_back(correct_prob_near_lattice);

                std::cout << std::endl << "n = " << n << ", r = " << r << ", d = " << d << std::endl;
                std::cout << std::fixed << correct_prob_uniform << " correctness probability, uniform sample" << std::endl;
                std::cout << std::fixed << correct_prob_near_lattice << " correctness probability, near-lattice sample" << std::endl;
            }
            plt::subplot2grid (n_vals.size(), r_vals.size(), plot_row, plot_col);
            plt::tight_layout();
            plt::named_plot("uniform sample", d_vals, correctness_uniform);
            plt::named_plot("near-lattice sample", d_vals, correctness_near_lattice);
            plt::axvline(d_calculated);
            plt::ylabel("Probability of correct decryption");
            plt::xlabel("d value");
            char title[40];
            sprintf(title, "n = %d, r = %.2f", n, r);
            plt::title(std::string (title));
            plt::legend();
            plot_col++;
        }
        plot_row++;
    }
    plt::show();
}

void quick_test() {
    int n = 128;
    double r = 0.1;
    double d_prime = exp(-M_PI * r * r) / 20.0;
    double d = (1.0 / 12.0 - d_prime) * n;
    int s = 10;
    int k = n + 10;
    int num_trials = 1000;

    Cryptosystem cryptosystem(n, k, r, s, d);
    std::cout << "Generating keys..." << std::endl;
    cryptosystem.generate_keys();
    std::cout << "Keys generated." << std::endl;

    double correct_prob_uniform = decryption_correctness_prob_uniform(cryptosystem, num_trials);
    double correct_prob_near_lattice = decryption_correctness_prob_near_lattice(cryptosystem, num_trials);
    std::cout << std::fixed << correct_prob_uniform << " correctness probability, uniform sample" << std::endl;
    std::cout << std::fixed << correct_prob_near_lattice << " correctness probability, near-lattice sample" << std::endl;
}

int main() {
    // Uncomment one of the following to run a test

    //generate_plots();
    //quick_test();

    return 0;
}
