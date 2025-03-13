#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK, TEST_CASE

#include <mywheel/map_adapter.hpp>  // for MapAdapter, key_iterator
#include <vector>

TEST_CASE("Test MapAdapter") {
    const auto V = std::vector<double>{0.6, 0.7, 0.8};
    const auto S = MapConstAdapter<std::vector<double>>{V};
    CHECK_EQ(S.size(), 3U);
    CHECK(S.contains(1));
    auto count = 0;
    for (const auto &p : S) {
        static_assert(sizeof(p) >= 0, "make compiler happy");
        ++count;
    }
    CHECK(count == 3);
}
