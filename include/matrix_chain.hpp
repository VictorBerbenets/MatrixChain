#ifndef MATRIX_CHAIN
#define MATRIX_CHAIN

#include <deque>
#include <iterator>
#include <utility>
#include <stack>

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
        chain_.insert(chain_.begin(), begin, end);
        // saving matrix sizes in order
        if (chain_.size()) {
            matrix_sizes_.push_back(front().nline());
            matrix_sizes_.push_back(front().ncolumn());
            for (size_type index = 1; index < chain_.size(); ++index) {
                matrix_sizes_.push_back(chain_[index].ncolumn());
            }
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

    reference front() { return chain_.front(); }
    reference back()  { return chain_.back(); }
    const_reference front() const { return chain_.front(); }
    const_reference back()  const { return chain_.back(); }

    check_pair push_back(const matrix_type& matrix) {
        if (insertion_check(matrix, InsertPos::Back)) {
            push_size(matrix, InsertPos::Back);
            chain_.push_back(matrix);
            return {std::prev(end()), true};
        }
        return {std::prev(end()), false};
    }

    check_pair push_back(matrix_type&& matrix) {
        if (insertion_check(matrix, InsertPos::Back)) {
            push_size(matrix, InsertPos::Back);
            chain_.push_back(std::move(matrix));
            return {std::prev(end()), true};
        }
        return {std::prev(end()), false};
    }

    check_pair push_front(const matrix_type& matrix) {
        if (insertion_check(matrix, InsertPos::Front)) {
            push_size(matrix, InsertPos::Front);
            chain_.push_front(matrix);
            return {begin(), true};
        }
        return {begin(), false};
    }

    check_pair push_front(matrix_type&& matrix) {
        if (insertion_check(matrix, InsertPos::Front)) {
            push_size(matrix, InsertPos::Front);
            chain_.push_front(std::move(matrix));
            return {chain_.begin(), true};
        }
        return {chain_.begin(), false};
    }

    std::vector<size_type> get_optimal_mul_order() const {
        if (empty()) {
            return {0};
        }
        auto range = matrix_sizes_.size() - 1;
        Matrix<size_type> cost_table{range + 1, range + 1, 0};
        Matrix<size_type> optimal_costs{range, range + 1, 0};

        for (size_type l = 2; l <= range; ++l) {
            for (size_type i = 1; i <= range - l + 1; ++i) {
                auto j = i + l - 1;
                cost_table[i][j] = __LONG_MAX__;
                for (auto k = i; k <= j - 1; ++k) {
                    auto q = cost_table[i][k] + cost_table[k + 1][j] +
                             matrix_sizes_[i-1] * matrix_sizes_[k] * matrix_sizes_[j];
                    std::cout << "q = " << q << std::endl;
                    if (q < cost_table[i][j]) {
                        cost_table[i][j]    = q;
                        optimal_costs[i][j] = k;
                    }
                }
            }
        }
        std::cout << "deque\n";
        for (auto val : matrix_sizes_) {
            std::cout << val << ' ';
        }
        std::cout << std::endl;

        std::cout << cost_table << std::endl;
        std::cout << optimal_costs << std::endl;

        std::vector<size_type> vec {};
        optimal_order(vec, optimal_costs, 1, 6);
        /*std::cout << "VECTOR\n";
        for (auto val : vec) {
            std::cout << val << std::endl;
        }*/
        return {};
    }

    void optimal_order(std::vector<size_type>& vec, Matrix<size_type>& m, size_type i, size_type j) const {
        if (i == j) {
           //std::cout << i << std::endl;
           //std::cout << "A" << i;
        } else {
            //std::cout << "(";
            optimal_order(vec, m, i, m[i][j]);
            //vec.push_back(i - 1);
            optimal_order(vec, m, m[i][j] + 1, j);
            //std::cout << i - 1;
            std::cout << "i = " << i << std::endl;
            std::cout << "j = " << j << std::endl;

           // vec.push_back(j - 2);
            //vec.push_back(j);
           // std::cout << ")";
        }
#if 0
        std::stack<size_type> ides {};

        for (size_type id = 0; id < m.size(); ++id) {
            if (i == j) {

            }
        }
#endif
    }

    size_type size() const noexcept { return chain_.size(); }
    bool empty() const noexcept { return chain_.empty(); }

    iterator begin() noexcept { return chain_.begin(); }
    iterator end()   noexcept { return chain_.end(); }
    const_iterator cbegin() const noexcept { return chain_.cbegin(); }
    const_iterator cend()   const noexcept { return chain_.cend(); }
    reverse_iterator rbegin() noexcept { return chain_.rbegin(); }
    reverse_iterator rend()   noexcept { return chain_.rend(); }
    const_reverse_iterator crbegin() const noexcept { return chain_.crbegin(); }
    const_reverse_iterator crend()   const noexcept { return chain_.crend(); }
private:
    bool insertion_check(const matrix_type& matrix, InsertPos pos) const {
        if (empty()) { return true; }

        switch(pos) {
            case InsertPos::Front:
                return chain_.front().nline() == matrix.ncolumn();
            case InsertPos::Back:
                return chain_.back().ncolumn() == matrix.nline();
            default: return false;
        }
    }

    void push_size(const matrix_type& matrix, InsertPos pos) {
        if (empty()) {
            matrix_sizes_.push_back(matrix.nline());
            matrix_sizes_.push_back(matrix.ncolumn());
        } else {
            if (pos == InsertPos::Front) {
                matrix_sizes_.push_front(matrix.nline());
            } else {
                matrix_sizes_.push_back(matrix.ncolumn());
            }
        }
    }

private:
    std::deque<matrix_type> chain_;
    std::deque<size_type> matrix_sizes_;
};

} // <--- namespace yLAB

#endif

