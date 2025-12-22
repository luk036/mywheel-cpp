#include <doctest/doctest.h>  // for ResultBuilder, CHECK, Expression_lhs
// #include <__config>            // for std
#include <cstdint>  // for int32_t, uint32_t
#include <memory>
#include <mywheel/bpqueue.hpp>  // for BPQueue
#include <mywheel/dllist.hpp>   // for Dllink
#include <type_traits>
#include <utility>  // for pair
#include <vector>   // for vector

using namespace std;

TEST_CASE("Test BPQueue 1") {
    auto bpq = BPQueue<int, int32_t>{-3, 3};
    auto node_a = Dllink<std::pair<int, uint32_t>>{std::make_pair(3, uint32_t(0))};
    bpq.append(node_a, 0);
    CHECK_EQ(bpq.get_max(), 0);
    CHECK(!bpq.is_empty());
    bpq.set_key(node_a, 0);
    CHECK_EQ(node_a.data.second, 4);
    bpq.popleft();
    CHECK(bpq.is_empty());
    CHECK_EQ(bpq.get_max(), -4);
}

TEST_CASE("Test BPQueue 2") {
    auto bpq = BPQueue<int, int32_t>{-3, 3};
    auto node_a = Dllink<std::pair<int, uint32_t>>{std::make_pair(3, uint32_t(0))};
    bpq.appendleft_direct(node_a);
    CHECK_EQ(bpq.get_max(), 0);
    bpq.increase_key(node_a, 1);
    CHECK_EQ(bpq.get_max(), 1);
    bpq.decrease_key(node_a, 1);
    CHECK_EQ(bpq.get_max(), 0);

    auto it = bpq.begin();
    auto& node_b = *it;
    bpq.decrease_key(node_a, 1);
    CHECK_EQ(node_b.data.second, 3);
    bpq.increase_key(node_a, 1);
    CHECK_EQ(node_b.data.second, 4);
    bpq.modify_key(node_a, 1);
    CHECK_EQ(node_b.data.second, 5);
    bpq.detach(node_a);
    CHECK_EQ(bpq.get_max(), -4);
}

TEST_CASE("Test BPQueue 3") {
    constexpr auto PMAX = 10;

    auto bpq1 = BPQueue<int, int32_t>{-PMAX, PMAX};
    auto bpq2 = BPQueue<int, int32_t>{-PMAX, PMAX};

    CHECK(bpq1.is_empty());

    auto node_d = Dllink<std::pair<int, uint32_t>>{std::make_pair(0, uint32_t(0))};
    auto node_e = Dllink<std::pair<int, uint32_t>>{std::make_pair(0, uint32_t(0))};
    auto node_f = Dllink<std::pair<int, uint32_t>>{std::make_pair(0, uint32_t(0))};

    CHECK(node_d.data.second == 0);

    bpq1.appendleft(node_e, 3);
    bpq1.append(node_f, -PMAX);
    bpq1.append(node_d, 5);

    bpq2.append(bpq1.popleft(), -6);  // node_d
    bpq2.append(bpq1.popleft(), 3);
    bpq2.append(bpq1.popleft(), 0);

    bpq2.modify_key(node_d, 15);
    bpq2.modify_key(node_d, -3);
    CHECK(bpq1.is_empty());
    CHECK(bpq2.get_max() == 6);

    auto nodelist = vector<Dllink<pair<int, uint32_t>>>(10);

    auto idx = 0U;
    for (auto& it : nodelist) {
        it.data.second = 2 * idx - 10;
        idx += 1;
    }
}

TEST_CASE("Test BPQueue get_max") {
    auto bpq = BPQueue<int, int32_t>{-3, 3};
    auto node_a = Dllink<std::pair<int, uint32_t>>{std::make_pair(3, uint32_t(0))};
    bpq.append(node_a, 0);
    CHECK_EQ(bpq.get_max(), 0);
    bpq.increase_key(node_a, 1);
    CHECK_EQ(bpq.get_max(), 1);
    bpq.decrease_key(node_a, 2);
    CHECK_EQ(bpq.get_max(), -1);
}
