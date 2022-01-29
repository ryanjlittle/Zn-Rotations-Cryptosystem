#include <iostream>

#include "cryptosystem.h"

int main() {

    Cryptosystem cryptosystem(128, 138, 0, 0.01, 10, 0.1, 10); // n=128, k=138, c=0, r=0.01, s=10, d=0.1, t=10
    cryptosystem.generate_keys();

    for (int i=0; i<10; i++) {
        arma::vec ctext = cryptosystem.encrypt(cryptosystem.getG(), false);
        bool decryption = cryptosystem.decrypt(cryptosystem.getB(), ctext);
        std::cout << "Expected false, returned " << std::boolalpha << decryption << std::endl;
    }

    for (int i=0; i<10; i++) {
        arma::vec ctext = cryptosystem.encrypt(cryptosystem.getG(), true);
        bool decryption = cryptosystem.decrypt(cryptosystem.getB(), ctext);
        std::cout << "Expected true, returned " << std::boolalpha << decryption << std::endl;
    }

    return 0;
}
