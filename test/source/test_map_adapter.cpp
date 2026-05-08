#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK, TEST_CASE

#include <mywheel/map_adapter.hpp>  // for MapAdapter, key_iterator
#include <vector>

TEST_CASE("Test MapAdapter") {
    const auto V = std::vector<double>{0.6, 0.7, 0.8};
    const auto S = MapConstAdapter<std::vector<double>>{V};
    CHECK_EQ(S.size(), 3U);
    CHECK(S.contains(1));
    auto count = 0;
    for ([[maybe_unused]] const auto& p : S) {
        ++count;
    }
    CHECK_EQ(count, 3);
}

TEST_CASE("Test MapAdapter operator[]") {
    const auto V = std::vector<double>{0.6, 0.7, 0.8};
    const auto S = MapConstAdapter<std::vector<double>>{V};
    CHECK_EQ(S[0], 0.6);
    CHECK_EQ(S[1], 0.7);
    CHECK_EQ(S[2], 0.8);
}
