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

TEST_CASE("Test ShiftArray operator[]") {
    ShiftArray<int> arr(std::vector<int>{1, 2, 3, 4, 5});
    arr.set_start(2);
    CHECK_EQ(arr[2], 1);
    CHECK_EQ(arr[3], 2);
    CHECK_EQ(arr[4], 3);
    CHECK_EQ(arr[5], 4);
    CHECK_EQ(arr[6], 5);
}

TEST_CASE("Test RepeatArray Stress Test") {
    const size_t large_size = 1000000;
    RepeatArray<int> arr(7, large_size);
    CHECK_EQ(arr.size(), large_size);
    for (size_t i = 0; i < large_size; ++i) {
        CHECK_EQ(arr[i], 7);
    }
    size_t count = 0;
    for (int i : arr) {
        ++count;
        CHECK_EQ(i, 7);
    }
    CHECK_EQ(count, large_size);
}

TEST_CASE("Test ShiftArray Stress Test") {
    const size_t large_size = 1000000;
    std::vector<int> vec(large_size);
    for (size_t i = 0; i < large_size; ++i) {
        vec[i] = static_cast<int>(i);
    }
    ShiftArray<int> arr(vec);
    size_t start_offset = large_size / 2;
    arr.set_start(start_offset);
    for (size_t key = start_offset; key < start_offset + arr.size(); ++key) {
        CHECK_EQ(arr[key], static_cast<int>(key - start_offset));
    }
}
