#include <iostream>
#include <string>

#include "end2end.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        throw std::runtime_error {"invalid number of arguments: expected 2, got" + std::to_string(argc)};
    }
    std::size_t tests_number = std::stol(argv[1]);

    testing::generator generator {tests_number};
    generator.generate_tests();
}

