#ifndef ZNROTATIONCRYPTOSYSTEM_SAMPLING_H
#define ZNROTATIONCRYPTOSYSTEM_SAMPLING_H

#include<vector>
#include<random>
#include<cmath>
#include<numeric>
#include<algorithm>

std::vector<int> sample_discrete_gaussian(int n, double c, double s, double t);

std::vector<std::vector<int>> sample_Zn_basis(int n, double c, double s, double t);


#endif //ZNROTATIONCRYPTOSYSTEM_SAMPLING_H
