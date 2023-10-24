#ifndef END2END_TESTING
#define END2END_TESTING

#include <vector>
#include <random>
#include <ctime>
#include <filesystem>

#include "matrix.hpp"

namespace testing {

namespace dirs {
    const std::string resource_dir = "../tests/end2end/resources/";
    const std::string tests_dir    = resource_dir + "tests/";
    const std::string ans_dir      = resource_dir + "answers/";
}

class generator {
    using size_type = std::size_t;
    using generator_type    = std::mt19937;
    using distribution_type = std::uniform_int_distribution<size_type>;
    using value_type        = size_type;

    static constexpr size_type MIN_MATRICES_NUMBER = 2;
    static constexpr size_type MAX_MATRICES_NUMBER = 200;
    static constexpr size_type MIN_MATRIX_SIZE = 1;
    static constexpr size_type MAX_MATRIX_SIZE = 100;

    void create_source_directory();
    value_type random_value(value_type min_val = MIN_MATRICES_NUMBER, value_type max_val = MAX_MATRICES_NUMBER);
    void generate_optimal_orders(size_type number);
    size_type fill_optimal_order(size_type i, size_type j, yLAB::Matrix<size_type>& lookup_table);
    void write_data_into_files(size_type test_number, size_type matrices_number, size_type ans);
public:
    generator(size_type tests_number);

    void generate_tests();
private:
    std::vector<size_type> matrices_sizes_;
    size_type tests_number_;
    generator_type generator_;

}; // <--- class generator

} // <--- namespace testing

#endif
