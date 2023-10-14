#ifndef MATRIX_CHAIN
#define MATRIX_CHAIN

#include <vector>

#include "matrix.hpp"

namespace yLAB {

template<numeric_type T>
class MatrixChain final {
public:
    using size_type              = std::size_t;
    using value_type             = T;
    using matrix_type            = Matrix<value_type>;
    using reference              = matrix_type&;
    using const_reference        = const matrix_type&;
    using pointer                = matrix_type*;
    using const_pointer          = const matrix_type*;
    using iterator               = typename std::vector<matrix_type>::iterator;
    using const_iterator         = typename std::vector<matrix_type>::const_iterator;
    using reverse_iterator       = typename std::vector<matrix_type>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<matrix_type>::const_reverse_iterator;


private:
    std::vector<matrix_type> chain_;
};

}; // <--- namespace yLAB

#endif

