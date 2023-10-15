#ifndef MATRIX_CHAIN
#define MATRIX_CHAIN

#include <deque>
#include <iterator>
#include <utility>

#include "matrix.hpp"

namespace yLAB {

template<numeric_type T>
class MatrixChain final {
public:
    using value_type             = T;
    using matrix_type            = Matrix<value_type>;
    using reference              = matrix_type&;
    using const_reference        = const matrix_type&;
    using pointer                = matrix_type*;
    using const_pointer          = const matrix_type*;
    using iterator               = typename std::deque<matrix_type>::iterator;
    using const_iterator         = typename std::deque<matrix_type>::const_iterator;
    using reverse_iterator       = typename std::deque<matrix_type>::reverse_iterator;
    using const_reverse_iterator = typename std::deque<matrix_type>::const_reverse_iterator;
    using size_type              = typename std::deque<matrix_type>::size_type;
private:
    using check_pair = std::pair<const_iterator, bool>;

    enum class InsertPos : char { Front = 1, Back = 2};
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
    
    constexpr check_pair push_back(const matrix_type& matrix) {
        if (insertion_check(matrix, InsertPos::Back)) {
            chain_.push_back(matrix);
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    constexpr check_pair push_back(matrix_type&& matrix) {
        if (insertion_check(matrix, InsertPos::Back)) {
            chain_.push_back(std::move(matrix));
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    constexpr check_pair push_front(const matrix_type& matrix) {
        if (insertion_check(matrix, InsertPos::Front)) {
            chain_.push_front(matrix);
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    constexpr check_pair push_front(matrix_type&& matrix) {
        if (insertion_check(matrix, InsertPos::Front)) {
            chain_.push_front(std::move(matrix));
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }
 
    size_type size() const noexcept { return chain_.size(); };
    bool empty() const noexcept { return chain_.empty(); };
private:
    bool insertion_check(const matrix_type& matrix, InsertPos pos) const {
        if (!size()) { return true; }

        switch(pos) {
            case InsertPos::Front:
                return chain_.front().nline() == matrix.ncolumn();
            case InsertPos::Back:
                return chain_.back().ncolumn() == matrix.nline();
            default: break;
        }
        return false; // we won't get here
    }

private:
    std::deque<matrix_type> chain_;
};

}; // <--- namespace yLAB

#endif

