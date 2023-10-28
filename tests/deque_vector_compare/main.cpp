#include <iostream>
#include <stdexcept>
#include <vector>
#include <chrono>

#include "matrix_chain.hpp"
#include "vector_chain.hpp"


std::vector<std::size_t> get_data(std::istream& is) {
    std::size_t ms_number {0};
    is >> ms_number;
    if (is.fail()) {
        throw std::runtime_error{"data reading error"};
    }

    std::vector<std::size_t> data{};
    data.reserve(ms_number);

    for (std::size_t count {0}; count < ms_number; ++count) {
        std::size_t tmp {0};
        is >> tmp;
        if (is.fail()) {
            throw std::runtime_error{"data reading error"};
        }
        data.push_back(tmp);
    }

    return data;
}

template<typename ChainType>
auto optimal_order_time(const ChainType& chain) {
    auto start     = std::chrono::high_resolution_clock::now();
    auto opt_order = chain.get_optimal_mul_order();
    auto end       = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    return duration;
}

template<typename ChainType>
auto effective_multiply_time(const ChainType& chain) {
    auto start  = std::chrono::high_resolution_clock::now();
    auto matrix = chain.effective_multiply();
    auto end    = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    return duration;
}

namespace colors {
    const std::string green = "\033[1;32m";
    const std::string blue  = "\033[1;36m";
    const std::string usual = "\033[0m";
}

void print_results(const std::string& container, const std::chrono::duration<double>& opt_time, const std::chrono::duration<double>& mul_time) {
    std::cout << colors::blue + container + colors::usual << ":" << std::endl;
    std::cout << colors::green + "Optimal order time:" + colors::usual << opt_time.count() << std::endl;
    std::cout << colors::green + "Effective multiply time:" + colors::usual << mul_time.count() << std::endl;
}

int main() {
    using def_type = int;

    auto data = get_data(std::cin);

    yLAB::MatrixChain<def_type> deque_chain;
    yLAB::MatrixChain_V<def_type> vector_chain;

    for (auto iter = data.begin(), end = std::prev(data.end()); iter != end; ++iter) {
        deque_chain.emplace_back(*iter, *(iter + 1), def_type {});
        vector_chain.emplace_back(*iter, *(iter + 1), def_type {});
    }

    print_results("deque implementation", optimal_order_time(deque_chain), effective_multiply_time(deque_chain));
    std::cout << std::endl;
    print_results("vector implementation", optimal_order_time(vector_chain), effective_multiply_time(vector_chain));

}