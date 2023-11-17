#include <doctest/doctest.h>  // for ResultBuilder, TestCase, CHECK, TEST_CASE

#include <mywheel/array_like.hpp>  // for RepeatArrat, key_iterator

TEST_CASE("Test RepeatArray") {
    RepeatArray<int> arr(1, 10);
    CHECK_EQ(arr[4], 1);
    size_t count = 0;
    for (int i : arr) {
        ++count;
        CHECK_EQ(i, 1);
    }
    CHECK_EQ(count, arr.size());
}

TEST_CASE("Test ShiftArray") {
    ShiftArray<int> arr(std::vector<int>{1, 2, 3, 4, 5});
    arr.set_start(2);
    CHECK_EQ(arr[4], 3);
    // size_t count = 0;
    // for (auto res : arr.items()) {
    //     ++count;
    //     CHECK_EQ(std::get<0>(res), std::get<1>(res) + 1);
    // }
    // CHECK_EQ(count, arr.size());
}
