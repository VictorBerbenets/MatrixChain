#ifndef MATRIX_CHAIN
#define MATRIX_CHAIN

#include <vector>
#include <iterator>

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
    
public:
    constexpr MatrixChain() = default;

    constexpr explicit MatrixChain(size_type size) {
        chain_.reserve(size);
    }

    template<std::forward_iterator ForwIter>
    constexpr MatrixChain(ForwIter begin, ForwIter end)
    : MatrixChain(std::distance(begin, end)) {
        chain_.insert(chain_.begin(), begin(), end());
    }

    constexpr MatrixChain(std::initializer_list<matrix_type> ls)
    : MatrixChain(ls.begin(), ls.end()) {}
    
    constexpr void add(const matrix_type& matrix) {
        chain_.push_back(std::forward<matrix_type>(matrix));
    }
    
private:
    std::vector<matrix_type> chain_;
};

}; // <--- namespace yLAB

#endif

