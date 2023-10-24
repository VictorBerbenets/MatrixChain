#include <gtest/gtest.h>

#include "matrix_chain.hpp"

using namespace yLAB;

TEST(Multiply, DirectMultiply1) {
    MatrixChain<int> chain {};

    ASSERT_EQ(chain.multiply(), (Matrix<int>{0, 0, 0}));
}

TEST(Multiply, DirectMultiply2) {
    MatrixChain<int> chain {};

    chain.emplace_back(1u, 1u, 1);
    ASSERT_EQ(chain.multiply(), (Matrix<int>{1, 1, 1}));
}

TEST(Multiply, DirectMultiply3) {
    MatrixChain<int> chain {};

    std::size_t j = 100;
    for (std::size_t i = 1; i <= 10; ++i) {
        chain.emplace_back(j ,i, 0);
        j = i;
    }
    ASSERT_EQ(chain.multiply(), (Matrix<int>{100u, 10, 0}));
}

TEST(Multiply, EffectiveMultiply1) {
    MatrixChain<int> chain {};

    ASSERT_EQ(chain.effective_multiply(), chain.multiply());
}


TEST(Multiply, EffectiveMultiply2) {
    MatrixChain<int> chain {};

    std::size_t j = 100;
    for (std::size_t i = 1; i <= 10; ++i) {
        chain.emplace_back(j ,i, 0);
        j = i;
    }
    ASSERT_EQ(chain.effective_multiply(), chain.multiply());
}

TEST(Multiply, EffectiveMultiply3) {
    MatrixChain<int> chain {};

    std::size_t j = 100;
    for (std::size_t i = 1; i <= 100; ++i) {
        chain.emplace_back(j ,i, 0);
        j = i;
    }
    ASSERT_EQ(chain.effective_multiply(), chain.multiply());
}

