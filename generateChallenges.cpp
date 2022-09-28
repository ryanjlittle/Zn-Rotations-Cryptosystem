#include "generateChallenges.h"

void generateChallenges(Cryptosystem cryptosystem, int num_challenges, std::string path) {

    std::filesystem::create_directories(path);
    std::cout.precision(30);

    cryptosystem.generate_keys();

    std::ofstream private_key_file(path + "/private_key");
    std::ofstream public_key_file(path + "/public_key");

    private_key_file << std::fixed << cryptosystem.getB();
    public_key_file << std::fixed << cryptosystem.getG_inv();

    private_key_file.close();
    public_key_file.close();

    osuCrypto::PRNG prng;
    prng.SetSeed(osuCrypto::sysRandomSeed());

    for (int i = 0; i < num_challenges; i++) {
        bool ptext = prng.getBit();

        VectorXm ctext = cryptosystem.encrypt(cryptosystem.getG_inv(), ptext);
        assert(cryptosystem.decrypt(cryptosystem.getB(), ctext) == ptext);

        std::ofstream plaintext_file(path + "/plaintext" + std::to_string(i));
        std::ofstream ciphertext_file(path + "/ciphertext" + std::to_string(i));

        mpfr::mpreal first = ctext(0);
        double first_dub = first.toDouble();

        plaintext_file << std::fixed << ptext;
        ciphertext_file << std::fixed << ctext;

        plaintext_file.close();
        ciphertext_file.close();
    }
}
