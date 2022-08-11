# Zn-Rotations-Cryptosystem

An implementation of the [BGPS cryptosystem](https://eprint.iacr.org/2021/1548.pdf), based on rotations of the $\mathbb{Z}^n$ lattice. Challenge public keys and ciphertexts are given in the `challenges` folder. Each challenge subfolder corresponds to a fixed $n$ value and decryption failure probability $p_f$ value. For example, the subfolder `n128_fp2-10` contains challenges for $n=128$ with $p_f=2^{-10}$. 

## Dependencies

The project relies on the following dependencies:
- [fplll](https://github.com/fplll/fplll) 
- [Eigen](https://eigen.tuxfamily.org)
- [CryptoTools](https://github.com/ladnir/cryptoTools)
- [MPFR](https://www.mpfr.org/)
- Python 3.10 (optional, only needed for generating plots)

By default, header files for Eigen unsupported modules are installed at `/usr/include/eigen3/unsupported/Eigen`. These should be moved or symlinked to `/usr/include/EigenUnsupported`. Alternatively you can change line 7 in `utils.h` and line 37 in `eigenmvn/eigenmvn.h` to match the location of your header files for Eigen unsupported modules.

## Building and running 

Install all dependencies above.

Clone the repo. Make sure you clone the submodule as well, which can be done with the `--recursive` flag.

`$ git clone --recursive https://github.com/ryanjlittle/Zn-Rotations-Cryptosystem`

The project can be built using Cmake:


`$ cmake . && cmake --build . --target ZnRotationCryptosystem`
