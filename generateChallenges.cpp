#include "generateChallenges.h"

void generateChallenges(Cryptosystem cryptosystem, int num_challenges, std::string path) {

    std::filesystem::create_directories(path);

    cryptosystem.generate_keys();

    std::ofstream private_key_file(path + "/private_key");
    std::ofstream public_key_file(path + "/public_key");

    private_key_file << std::fixed << cryptosystem.get_B();
    public_key_file << std::fixed << cryptosystem.get_G_inv();


    private_key_file.close();
    public_key_file.close();

    osuCrypto::PRNG prng;
    prng.SetSeed(osuCrypto::sysRandomSeed());

    for (int i = 0; i < num_challenges; i++) {
        bool ptext = prng.getBit();

        VectorXm ctext = cryptosystem.encrypt(cryptosystem.get_G_inv(), ptext);
        assert(cryptosystem.decrypt(cryptosystem.get_B(), ctext) == ptext);

        std::ofstream plaintext_file(path + "/plaintext" + std::to_string(i));
        std::ofstream ciphertext_file(path + "/ciphertext" + std::to_string(i));

        ciphertext_file.precision(30);

        plaintext_file << std::fixed << ptext;
        ciphertext_file << std::fixed << ctext;

        plaintext_file.close();
        ciphertext_file.close();
    }
}

void generateChallengesRepCode(Cryptosystem cryptosystem, int num_challenges, int rep, std::string path) {
    osuCrypto::PRNG prng;
    prng.SetSeed(osuCrypto::sysRandomSeed());

    for (int i = 0; i < num_challenges; i++) {

        std::cout << "Generating challenge " << i << " for n=" << cryptosystem.get_n() << ", d/n=" << cryptosystem.get_d() / cryptosystem.get_n() << std::endl;
        bool ptext = prng.getBit();

        MatrixXm ctext = cryptosystem.encrypt_rep_code(cryptosystem.get_G_inv(), ptext, rep);
        assert(cryptosystem.decrypt_rep_code(cryptosystem.get_B(), ctext, rep) == ptext);

        std::ofstream plaintext_file(path + "/plaintext" + std::to_string(i));
        std::ofstream ciphertext_file(path + "/ciphertext" + std::to_string(i));

        ciphertext_file.precision(30);

        plaintext_file << std::fixed << ptext;
        ciphertext_file << std::fixed << ctext;

        plaintext_file.close();
        ciphertext_file.close();
    }

}
