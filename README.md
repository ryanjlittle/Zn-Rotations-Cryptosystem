# Zn-Rotations-Cryptosystem

An implementation of the [BGPS cryptosystem](https://eprint.iacr.org/2021/1548.pdf), based on rotations of the $\mathbb{Z}^n$ lattice.

## Dependencies

You must separately install the following dependencies:
- [fplll](https://github.com/fplll/fplll) 
- [Eigen](https://eigen.tuxfamily.org)
- [MPFR](https://www.mpfr.org/)
- Python 3.10 (optional, only needed for generating plots)

## Building and running 

Clone the repo. Make sure you clone the submodule as well, which can be done with the `--recursive` flag.

`$ git clone --recursive https://github.com/ryanjlittle/Zn-Rotations-Cryptosystem`

The project can be built using Cmake:

`$ cmake --build . --target ZnRotationCryptosystem.`
