#include <doctest/doctest.h>  // for ResultBuilder, CHECK, Expr...
// #include <__config>                        // for std
#include <cinttypes>          // for uint8_t
#include <mywheel/robin.hpp>  // for Robin, Robin<>::iterable_w...
#include <utility>            // for pair

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
