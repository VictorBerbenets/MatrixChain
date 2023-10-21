#include <iostream>
#include <iterator>
#include <vector>
#include <stdexcept>

#include "matrix_chain.hpp"

std::vector<std::size_t> get_data(std::istream& is) {
    std::size_t ms_number {0};
    is >> ms_number;
    if (!is.good()) {
        throw std::runtime_error{"data reading error"};
    }

    std::vector<std::size_t> data{};
    data.reserve(ms_number);
    for (std::size_t count = 0; count < ms_number; ++count) {
        std::size_t tmp {0};
        is >> tmp;
        data.push_back(tmp);
    }
    return data;
}

int main() {
    using def_type = int;

    yLAB::MatrixChain<def_type> chain {};
    auto data = get_data(std::cin);
    
    if (data.empty()) { return 0; }
    for (auto iter = data.begin(), end = std::prev(data.end()); iter != end; ++iter) {
        chain.emplace_back(*iter, *(iter + 1), def_type {});
    }
    
    for (auto val : chain.get_optimal_mul_order()) {
        std::cout << val << ' ';
    }
    std::cout << std::endl;
}

