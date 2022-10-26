//
// Created by ryan on 10/24/22.
//

#ifndef ZNROTATIONCRYPTOSYSTEM_LLLATTACK_H
#define ZNROTATIONCRYPTOSYSTEM_LLLATTACK_H

#include "cryptosystem.h"

#define MULT_CONST 1e10

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
    LLLattack(Cryptosystem cryptosystem);
    bool decrypt(VectorXm c);
};


#endif //ZNROTATIONCRYPTOSYSTEM_LLLATTACK_H
