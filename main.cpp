#include <iostream>
#include "sampling.h"

int main() {
    std::vector<std::vector<int>> basis = sample_Zn_basis(5, 0, 5, 10);

    for (std::vector<int> vec : basis) {
        for (int i: vec) {
            std::cout << i << "\t";
        }
        std::cout << std::endl;
    }
    return 0;
}
