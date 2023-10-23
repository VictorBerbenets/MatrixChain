#include <iostream>
#include <vector>
#include <climits>
#include <fstream>
#include <algorithm>

#include "end2end.hpp"
#include "matrix.hpp"

namespace testing {

void generator::create_source_directory() {
    using namespace std::filesystem;
    directory_entry resource_dir(dirs::resource_dir);

    if (!resource_dir.is_directory()) {
        create_directory(dirs::resource_dir);
        create_directory(dirs::tests_dir);
        create_directory(dirs::ans_dir);
    } else {
        const path tests_path{dirs::tests_dir};
        const path answs_path{dirs::ans_dir};

        for (auto& dir_iter : directory_iterator{tests_path}) {
            remove(dir_iter.path());
        }
        for (auto& dir_iter : directory_iterator{answs_path}) {
            remove(dir_iter.path());
        }
    }
}
void generator::generate_optimal_orders(size_type test_number) {
    size_type matrices_number = random_value();

    matrices_sizes_.reserve(matrices_number);
    for (size_type count = 0; count < matrices_number; ++count) {
        matrices_sizes_.push_back(random_value(MIN_MATRICES_NUMBER, MAX_MATRICES_NUMBER));
    }

    auto size = matrices_sizes_.size();
    yLAB::Matrix<size_type> lookup_table {size + 1, size + 1, 0};

    write_data_into_files(test_number, matrices_number, fill_optimal_order(0, size - 1, lookup_table));
}

generator::size_type generator::fill_optimal_order(size_type i, size_type j, yLAB::Matrix<size_type>& lookup_table) {
    if (j <= i + 1) {
        return 0;
    }
    auto min = __LONG_MAX__;

    if (lookup_table[i][j] == 0) {
        for (size_type k = i + 1; k <= j - 1; k++) {
            auto cost = fill_optimal_order(i, k, lookup_table);
            cost += fill_optimal_order(k, j, lookup_table);
            cost += matrices_sizes_[i] *matrices_sizes_[k] * matrices_sizes_[j];

            if (cost < min) {
                min = cost;
            }
        }
        lookup_table[i][j] = min;
    }
    return lookup_table[i][j];
}

void generator::write_data_into_files(size_type test_number, size_type matrices_number, size_type ans) {
    std::string test_name = "test" + std::to_string(test_number) + ".txt";
    std::string ans_name  = "ans" + std::to_string(test_number) + ".txt";
    std::ofstream test_file {dirs::tests_dir + test_name};
    std::ofstream ans_file  {dirs::ans_dir + ans_name};

    test_file << matrices_number << std::endl;
    for (auto val : matrices_sizes_) {
        test_file << val << ' ';
    }
    test_file << std::endl;

    ans_file << ans << std::endl;

    matrices_sizes_.clear();
}

generator::value_type generator::random_value(value_type min_val, value_type max_val) {
    distribution_type distr(min_val, max_val);
    return distr(generator_);
}

generator::generator(size_type tests_number)
: tests_number_ {tests_number} {
    generator_.seed( static_cast<size_type>(std::time(nullptr)) );
}

} // <--- namespace testing