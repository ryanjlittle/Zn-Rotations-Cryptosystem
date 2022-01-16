#include <iostream>
#include "sampling.h"
#include "fplll_utils.h"

int main() {
    // NOTE: this does not actually create generating sets of Z_n yet
    std::vector<std::vector<int>> sampled_generating_set = sample_Zn_generating_set(5, 0, 10, 10);
    fplll::ZZ_mat<mpz_t> generating_set = std_vector_to_fplll_ZZ_mat(sampled_generating_set);

    std::cout << "Sampled vectors:\n";
    generating_set.print_comma(std::cout);

    fplll::lll_reduction(generating_set, fplll::LLL_DEF_DELTA, fplll::LLL_DEF_ETA,
                         fplll::LM_FAST, fplll::FT_DEFAULT, 0, fplll::LLL_DEFAULT);

    std::cout << "LLL reduced vectors:\n";
    generating_set.print_comma(std::cout);

    return 0;
}