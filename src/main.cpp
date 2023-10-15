#include <iostream>

#include "matrix_chain.hpp"

int main() {
    yLAB::MatrixChain<int> chain{};

    yLAB::Matrix<int> m1 {2, 2, {1, 2, 3, 4}};
    yLAB::Matrix<int> m2 {2, 2, {1, 2, 3, 4}};
    yLAB::Matrix<int> m3 {2, 2, {1, 2, 3, 4}};
    yLAB::Matrix<int> m4 {3, 2, {1, 3, 4, 5, 6, 5}};

    chain.push_back(m1);
    chain.push_back(std::move(m2));
    chain.push_front(m3);
    chain.push_front(std::move(m4));

    std::cout << "size = " << chain.size() << std::endl;
}

