
#include "end2end.hpp"

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

generator::value_type generator::random_value(value_type min_val, value_type max_val) {
    distribution_type distr(min_val, max_val);
    return distr(generator_);
}

generator::generator(size_type tests_number, size_type keys_number, size_type queries_number)
: tests_number_ {tests_number} {
    generator_.seed( static_cast<size_type>(std::time(nullptr)) );
}

} // <--- namespace testing