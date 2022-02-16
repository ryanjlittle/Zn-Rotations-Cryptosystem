#include <iostream>
#include "cryptosystem.h"

int main() {

    int s = 10;
    for (int n = 2; n<256; n*=2) {
        std::cout << "\nn = " << n << std::endl;
        int k = n + 10;
        double r = sqrt((log(n) - pow(log(log(n)), 2)) / M_PI);
        //r *= 0.2;
        double d = n / 12 - n * exp(-M_PI * pow(r, 2)) / (4 * pow(M_PI, 2));
        Cryptosystem cryptosystem(n, k, r, s, d, 10);
        cryptosystem.generate_keys();

        int correct_decryptions = 0;
        for (int i = 0; i < 1000; i++) {
            arma::vec ctext = cryptosystem.encrypt(cryptosystem.getG(), false);
            bool decryption = cryptosystem.decrypt(cryptosystem.getB(), ctext);
            if (!decryption) {
                correct_decryptions++;
            }
        }
        std::cout << correct_decryptions << "/1000 correct decryptions (near-lattice-point sample)" << std::endl;

        correct_decryptions = 0;
        for (int i = 0; i < 1000; i++) {
            arma::vec ctext = cryptosystem.encrypt(cryptosystem.getG(), true);
            bool decryption = cryptosystem.decrypt(cryptosystem.getB(), ctext);
            if (decryption) {
                correct_decryptions++;
            }
        }
        std::cout << correct_decryptions << "/1000 correct decryptions (uniform sample)" << std::endl;
    }
    return 0;
}
