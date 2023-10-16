#ifndef MATRIX_CHAIN
#define MATRIX_CHAIN

#include <deque>
#include <iterator>
#include <utility>
#include <unordered_map>

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

    explicit MatrixChain(size_type size) {
        chain_.reserve(size);
        matrix_sizes_.reserve(++size);
    }

    template<std::forward_iterator ForwIter>
    MatrixChain(ForwIter begin, ForwIter end)
    : MatrixChain(std::distance(begin, end)) {
        chain_.insert(chain_.begin(), begin(), end());
        if (chain_.size()) {
            matrix_sizes_.insert(begin(), front().nline(), front().ncolumn());
        }
        for (size_type index = 1; index < chain_.size(); ++index) {
            matrix_sizes_.push_back(chain_[index].ncolumn());
        }
    }

    MatrixChain(std::initializer_list<matrix_type> ls)
    : MatrixChain(ls.begin(), ls.end()) {}

    reference operator[](size_type id) {
        return chain_[id];
    }

    const_reference operator[](size_type id) const {
        return chain_[id];
    }

    reference front() { return chain_.front(); };
    reference back()  { return chain_.back(); };
    const_reference front() const { return chain_.front(); };
    const_reference back()  const { return chain_.back(); };

    check_pair push_back(const matrix_type& matrix) {
        if (insertion_check(matrix, InsertPos::Back)) {
            matrix_sizes_.push_back(matrix.ncolumn());
            chain_.push_back(matrix);
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    check_pair push_back(matrix_type&& matrix) {
        if (insertion_check(matrix, InsertPos::Back)) {
            matrix_sizes_.push_back(matrix.ncolumn());
            chain_.push_back(std::move(matrix));
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    check_pair push_front(const matrix_type& matrix) {
        if (insertion_check(matrix, InsertPos::Front)) {
            matrix_sizes_.push_front(matrix.nline());
            chain_.push_front(matrix);
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    check_pair push_front(matrix_type&& matrix) {
        if (insertion_check(matrix, InsertPos::Front)) {
            matrix_sizes_.push_front(matrix.nline());
            chain_.push_front(std::move(matrix));
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    std::vector<size_type> get_optimal_mul_order() const {

        std::vector<size_type> sizes_matrices {};
        sizes_matrices.reserve(chain_.size());
        for (size_type id {0}; auto val : chain_) {
           // sizes_matrices[id] = val.
        }

    }

    size_type size() const noexcept { return chain_.size(); };
    bool empty() const noexcept { return chain_.empty(); };

    iterator begin() noexcept { return chain_.begin(); };
    iterator end() noexcept { return chain_.end(); };
    const_iterator cbegin() noexcept { return chain_.cbegin(); };
    const_iterator cend() noexcept { return chain_.cend(); };
    reverse_iterator rbegin() noexcept { return chain_.rbegin(); };
    reverse_iterator rend() noexcept { return chain_.rend(); };
    const_reverse_iterator crbegin() noexcept { return chain_.rbegin(); };
    const_reverse_iterator crend() noexcept { return chain_.crend(); };
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
    std::deque<size_type> matrix_sizes_;
};

}; // <--- namespace yLAB

#endif

