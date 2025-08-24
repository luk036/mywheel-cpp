#include <doctest/doctest.h>  // for ResultBuilder, CHECK, Expr...
// #include <__config>                        // for std
#include <cinttypes>           // for uint8_t
#include <mywheel/dllist.hpp>  // for Dllist, DllIterator, oper...
#include <mywheel/robin.hpp>   // for Robin, fun::Robin<>::iterable_w...
#include <utility>             // for pair

using namespace std;

TEST_CASE("Test dllist") {
    auto L1 = Dllist<std::pair<int, int>>{std::make_pair(0, 0)};
    auto L2 = Dllist<std::pair<int, int>>{std::make_pair(0, 0)};
    auto d = Dllink<std::pair<int, int>>{std::make_pair(0, 0)};
    auto e = Dllink<std::pair<int, int>>{std::make_pair(0, 0)};
    auto f = Dllink<std::pair<int, int>>{std::make_pair(0, 0)};
    CHECK(L1.is_empty());

    L1.appendleft(e);
    CHECK(!L1.is_empty());

    L1.appendleft(f);
    L1.append(d);
    L2.append(L1.pop());
    L2.append(L1.popleft());
    CHECK(!L1.is_empty());
    CHECK(!L2.is_empty());

    auto count = 0U;
    for (const auto& _ : L2) {
        [[maybe_unused]] auto& dummy = _;
        count += 1;
    }
    CHECK(count == 2);
}

TEST_CASE("Test dllist pop") {
    auto L1 = Dllist<std::pair<int, int>>{std::make_pair(0, 0)};
    auto d = Dllink<std::pair<int, int>>{std::make_pair(1, 1)};
    auto e = Dllink<std::pair<int, int>>{std::make_pair(2, 2)};
    auto f = Dllink<std::pair<int, int>>{std::make_pair(3, 3)};

    L1.append(d);
    L1.append(e);
    L1.append(f);

    auto count = 0;
    for (const auto& _ : L1) {
        [[maybe_unused]] auto& dummy = _;
        count++;
    }
    CHECK(count == 3);

    auto& popped = L1.pop();
    CHECK(popped.data.first == 3);

    count = 0;
    for (const auto& _ : L1) {
        [[maybe_unused]] auto& dummy = _;
        count++;
    }
    CHECK(count == 2);

    auto& popped_left = L1.popleft();
    CHECK(popped_left.data.first == 1);

    count = 0;
    for (const auto& _ : L1) {
        [[maybe_unused]] auto& dummy = _;
        count++;
    }
    CHECK(count == 1);

    L1.pop();
    CHECK(L1.is_empty());
}

TEST_CASE("Test Robin") {
    fun::Robin<uint8_t> rr(6U);
    auto count = 0U;
    for (const auto& _i : rr.exclude(2)) {
        static_assert(sizeof _i >= 0, "make compiler happy");
        count += 1;
    }
    CHECK(count == 5);
}
