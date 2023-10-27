#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK, TEST_CASE

#include <mywheel/array_like.hpp>  // for RepeatArrat, key_iterator

TEST_CASE("Test RepeatArray") {
    RepeatArray arr(1, 10);
    CHECK_EQ(arr[4], 1);
    size_t count = 0;
    for (int i : arr) {
        ++count;
        CHECK_EQ(i, 1);
    }
    CHECK_EQ(count, arr.size());
}
