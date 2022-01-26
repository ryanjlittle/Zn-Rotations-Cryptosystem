#include <iostream>

#include "key_pair.h"


int main() {

    KeyPair key_pair(5, 10, 0, 10, 10);
    key_pair.generate_keys();
    key_pair.getB().print();
    key_pair.getG().print();

    return 0;
}
