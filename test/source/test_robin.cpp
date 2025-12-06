#include <doctest/doctest.h>  // for ResultBuilder, CHECK, Expr...
#include <algorithm>          // for std::find
#include <cinttypes>          // for uint8_t
#include <mywheel/robin.hpp>  // for Robin, Robin<>::iterable_w...
#include <numeric>            // for std::iota
#include <random>             // for std::mt19937, std::random_device, std::uniform_int_distribution
#include <utility>            // for pair
#include <vector>             // for std::vector

using namespace std;

TEST_CASE("Test Robin") {
    const fun::Robin<uint8_t> rr(6U);
    auto count = 0U;
    for ([[maybe_unused]] auto _i : rr.exclude(2)) {
        static_assert(sizeof _i >= 0, "make compiler happy");
        count += 1;
    }
    CHECK(count == 5);
}

TEST_CASE("Test Robin exclude") {
    const fun::Robin<uint8_t> rr(6U);
    auto count = 0U;
    auto sum = 0U;
    for (auto i : rr.exclude(2)) {
        count += 1;
        sum += i;
    }
    CHECK(count == 5);
    CHECK(sum == 0 + 1 + 3 + 4 + 5);
}

TEST_CASE("Test Robin Stress Test - Exclude and Iterate") {
    const int num_parts = 1000;
    fun::Robin<int> robin_stress(num_parts);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, num_parts - 1);

    const int num_iterations = 100;
    for (int iter = 0; iter < num_iterations; ++iter) {
        int excluded_part = distrib(gen);

        std::vector<int> expected_elements;
        for (int i = 0; i < num_parts; ++i) {
            if (i != excluded_part) {
                expected_elements.push_back(i);
            }
        }

        std::vector<int> actual_elements;
        for (int i : robin_stress.exclude(excluded_part)) {
            actual_elements.push_back(i);
        }
        std::sort(actual_elements.begin(), actual_elements.end());

        CHECK(actual_elements.size() == num_parts - 1);
        CHECK(actual_elements == expected_elements);
    }
}
