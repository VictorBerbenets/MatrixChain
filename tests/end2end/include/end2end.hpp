#ifndef END2END_TESTING
#define END2END_TESTING

#include <vector>
#include <random>
#include <ctime>
#include <filesystem>

namespace testing {

namespace dirs {
    const std::string resource_dir = "../tests/end2end/resources";
    const std::string tests_dir    = "../tests/end2end/resources/tests/";
    const std::string ans_dir      = "../tests/end2end/resources/answers/";
}

class generator {
    using size_type = std::size_t;
    using generator_type    = std::mt19937;
    using distribution_type = std::uniform_int_distribution<size_type>;
    using value_type        = size_type;

    void create_source_directory();
    value_type random_value(value_type min_val, value_type max_val);

public:
    generator(size_type tests_number, size_type keys_number, size_type queries_number);

    void generate_tests() {
        create_source_directory();

        for (size_type counter = 1; counter <= tests_number_; ++counter) {
            //generate_optimal_orders(counter);
        }
    }
private:
    std::vector<size_type> effective_order_;
    size_type tests_number_;
    generator_type generator_;

}; // <--- class generator

} // <--- namespace testing

#endif
