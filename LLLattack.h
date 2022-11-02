//
// Created by ryan on 10/24/22.
//

#ifndef ZNROTATIONCRYPTOSYSTEM_LLLATTACK_H
#define ZNROTATIONCRYPTOSYSTEM_LLLATTACK_H

#include "cryptosystem.h"

#define MULT_CONST 1e15

class LLLattack {

private:
    /**
     *  Public key
     */
    MatrixXm G_inv;

    /**
     * Dimension
     */
     int n;

    /**
     * Distance threshold used in decryption.
     */
    double d;

    /**
     *  Matrix L such that L * L^T = G. Computed via Cholesky decompostion
     */
    MatrixXm L;

public:
    /**
     * Constructor
     * @param cryptosystem Cryptosystem to be attacked. Keys must be generated before running attack.
     */
    LLLattack(Cryptosystem cryptosystem);

    /**
     * Run attack on given ciphertext c.
     * @param c
     * @return
     */
    bool decrypt(VectorXm c, int block_size);
};


#endif //ZNROTATIONCRYPTOSYSTEM_LLLATTACK_H
