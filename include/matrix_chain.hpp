#ifndef MATRIX_CHAIN
#define MATRIX_CHAIN

#include <deque>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include <list>

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
    using iterator               = std::deque<matrix_type>::iterator;
    using const_iterator         = std::deque<matrix_type>::const_iterator;
    using reverse_iterator       = std::deque<matrix_type>::reverse_iterator;
    using const_reverse_iterator = std::deque<matrix_type>::const_reverse_iterator;
    using size_type              = std::deque<matrix_type>::size_type;
private:
    using check_pair   = std::pair<const_iterator, bool>;
    using optimal_mmap = std::unordered_multimap<size_type, size_type>;
    using optimal_pair = std::pair<std::vector<size_type>, size_type>;
    using mul_map      = std::map<size_type, matrix_type>;
    using mul_iterator = mul_map::iterator;

    enum class InsertPos : char {Front, Back};
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
        if (insertion_check(matrix.nline(), InsertPos::Back)) {
            push_size(matrix.nline(), matrix.ncolumn(), InsertPos::Back);
            chain_.push_back(matrix);
            return {std::prev(end()), true};
        }
        return {std::prev(end()), false};
    }

    check_pair push_back(matrix_type&& matrix) {
        if (insertion_check(matrix.nline(), InsertPos::Back)) {
            push_size(matrix.nline(), matrix.ncolumn(), InsertPos::Back);
            chain_.push_back(std::move(matrix));
            return {std::prev(end()), true};
        }
        return {std::prev(end()), false};
    }

    check_pair push_front(const matrix_type& matrix) {
        if (insertion_check(matrix.ncolumn(), InsertPos::Front)) {
            push_size(matrix.nline(), matrix.ncolumn(), InsertPos::Front);
            chain_.push_front(matrix);
            return {begin(), true};
        }
        return {begin(), false};
    }

    check_pair push_front(matrix_type&& matrix) {
        if (insertion_check(matrix.ncolumn(), InsertPos::Front)) {
            push_size(matrix.nline(), matrix.ncolumn(), InsertPos::Front);
            chain_.push_front(std::move(matrix));
            return {begin(), true};
        }
        return {begin(), false};
    }

    template<typename... Args>
    check_pair emplace_back(size_type n_line, size_type n_column, Args&&... args) {
        if (insertion_check(n_line, InsertPos::Back)) {
            push_size(n_line, n_column, InsertPos::Back);
            chain_.emplace_back(n_line, n_column, std::forward<Args>(args)...);
            return {std::prev(end()), true};
        }
        return {std::prev(end()), false};
    }

    template<typename... Args>
    check_pair emplace_front(size_type n_line, size_type n_column, Args&&... args) {
        if (insertion_check(n_column, InsertPos::Front)) {
            push_size(n_line, n_column, InsertPos::Front);
            chain_.emplace_front(n_line, n_column, std::forward<Args>(args)...);
            return {begin(), true};
        }
        return {begin(), false};
    }

    void pop_back()  {     // if container is empty --> got UB
        if (size() == 1) {
            matrix_sizes_.clear();
        } else {
            matrix_sizes_.pop_back();
        }
        chain_.pop_back();
    }

    void pop_front() {    // if container is empty --> got UB
        if (size() == 1) {
            matrix_sizes_.clear();
        } else {
            matrix_sizes_.pop_front();
        }
        chain_.pop_front();
    }

    void clear() noexcept {
        matrix_sizes_.clear();
        chain_.clear();
    }

    optimal_pair get_optimal_mul_order() const {
        if (empty()) {
            return {};
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
                    if (q < cost_table[i][j]) {
                        cost_table[i][j]    = q;
                        optimal_costs[i][j] = k;
                    }
                }
            }
        }

        std::vector<size_type> data {};
        data.reserve(size());
        optimal_mmap mmap {};
        optimal_order(mmap, data, optimal_costs, 1, size());

        return {data, cost_table[1][size()]};
    }

    matrix_type effective_multiply() const {
        if (empty())     { return {0, 0, 0}; }
        if (size() == 1) { return front();   }

        auto order = get_optimal_mul_order().first;
        std::list<size_type> not_used_places(order.begin(), order.end());
        not_used_places.push_back(order.size());

        mul_map used_places {};
        for (auto val : order) {
            auto next_matrix_it = used_places.upper_bound(val);

            auto prev_matrix_it = used_places.end();
            for (auto it = used_places.begin(); it->first < val && it != used_places.end(); ++it) {
                prev_matrix_it = it;
            }

            auto next_matrix = neighbour_matrix(not_used_places, used_places, next_matrix_it, val + 1);
            auto prev_matrix = neighbour_matrix(not_used_places, used_places, prev_matrix_it, val);

            not_used_places.remove(val);
            not_used_places.remove(val + 1);

            used_places.emplace(val, prev_matrix * next_matrix);
        }
        return used_places.find(order.back())->second;
    }

    matrix_type multiply() const {
        if (empty()) {
            return matrix_type {0, 0, 0};
        }

        auto tmp = front();
        for (auto it = std::next(cbegin()), end = cend(); it != end; ++it) {
            tmp = tmp * (*it);
        }
        return tmp;
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
    void optimal_order(optimal_mmap& mmap, std::vector<size_type>& data, Matrix<size_type>& m, size_type i, size_type j) const {
        if (i == j) {
            return ;
        }

        optimal_order(mmap, data, m, i, m[i][j]);
        optimal_order(mmap, data, m, m[i][j] + 1, j);

        if (j - i == 1) {
            data.push_back(i - 1);
        } else {
            auto lower_range = mmap.equal_range(i);
            auto max_lower   = std::max_element(lower_range.first, lower_range.second);

            if (max_lower == lower_range.second) {
                data.push_back(i - 1);
            } else {
                data.push_back(max_lower->second - 1);
            }
        }
        mmap.emplace(i, j);
    }

    matrix_type neighbour_matrix(std::list<size_type>& not_used_places, mul_map& used_places, mul_iterator neighbour_it, size_type id) const {
        if (std::find(not_used_places.begin(), not_used_places.end(), id) != not_used_places.end()) {
            return chain_[id];
        }
        auto tmp = neighbour_it->second;
        used_places.erase(neighbour_it);

        return tmp;
    }

    bool insertion_check(size_type dimension, InsertPos pos) const {
        if (empty()) { return true; }

        switch(pos) {
            case InsertPos::Front:
                return chain_.front().nline() == dimension;
            case InsertPos::Back:
                return chain_.back().ncolumn() == dimension;
            default: return false;
        }
    }

    void push_size(size_type n_line, size_type n_column, InsertPos pos) {
        if (empty()) {
            matrix_sizes_.push_back(n_line);
            matrix_sizes_.push_back(n_column);
        } else {
            if (pos == InsertPos::Front) {
                matrix_sizes_.push_front(n_line);
            } else {
                matrix_sizes_.push_back(n_column);
            }
        }
    }

private:
    std::deque<matrix_type> chain_;
    std::deque<size_type> matrix_sizes_;
};

} // <--- namespace yLAB

#endif
