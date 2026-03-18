#ifdef RAPIDCHECK_H
#    include <rapidcheck.h>

#    include "mywheel/bpqueue.hpp"
#    include "mywheel/dllist.hpp"
#    include "mywheel/robin.hpp"

// Simple test to verify rc namespace is recognized
TEST_CASE("Property-based test: Simple rapidcheck test") {
    rc::check([]() {
        auto x = *rc::gen::inRange(0, 100);
        RC_ASSERT(x >= 0 && x < 100);
    });
}

// ============================================================================
// BPQueue Property-Based Tests
// ============================================================================

TEST_CASE("Property-based test: BPQueue maintains max key correctly") {
    rc::check([]() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(-10, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key, min_key + 20));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        std::vector<mywheel::Dllink<std::pair<int, uint32_t>>> items;
        auto num_items = static_cast<size_t>(*rc::gen::inRange(1, 50));

        for (size_t i = 0; i < num_items; ++i) {
            items.emplace_back();
            auto key = static_cast<int32_t>(*rc::gen::inRange(min_key, max_key + 1));
            items[i].data = std::make_pair(static_cast<int>(i), 0);
            bpq.appendleft(items[i], key);
        }

        auto expected_max = min_key - 1;
        for (size_t i = 0; i < num_items; ++i) {
            auto key = min_key + static_cast<int32_t>(items[i].data.second);
            if (key > expected_max) {
                expected_max = key;
            }
        }

        RC_ASSERT(bpq.get_max() == expected_max);
    });
}

TEST_CASE("Property-based test: BPQueue is_empty after clear") {
    rc::check("After clear(), is_empty() returns true", []() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(0, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key, min_key + 20));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        std::vector<mywheel::Dllink<std::pair<int, uint32_t>>> items;
        auto num_items = static_cast<size_t>(*rc::gen::inRange(1, 50));

        for (size_t i = 0; i < num_items; ++i) {
            items.emplace_back();
            auto key = static_cast<int32_t>(*rc::gen::inRange(min_key, max_key + 1));
            items[i].data = std::make_pair(static_cast<int>(i), 0);
            bpq.appendleft(items[i], key);
        }

        bpq.clear();
        RC_ASSERT(bpq.is_empty());
    });
}

TEST_CASE("Property-based test: BPQueue popleft returns items in descending key order") {
    rc::check("popleft() returns items from highest to lowest key", []() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(0, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key, min_key + 20));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        std::vector<mywheel::Dllink<std::pair<int, uint32_t>>> items;
        auto num_items = static_cast<size_t>(*rc::gen::inRange(10, 50));

        std::vector<int32_t> keys;
        for (size_t i = 0; i < num_items; ++i) {
            items.emplace_back();
            auto key = static_cast<int32_t>(*rc::gen::inRange(min_key, max_key + 1));
            items[i].data = std::make_pair(static_cast<int>(i), 0);
            bpq.appendleft(items[i], key);
            keys.push_back(key);
        }

        std::sort(keys.begin(), keys.end(), std::greater<int32_t>());

        size_t extracted_count = 0;
        while (!bpq.is_empty()) {
            auto& item = bpq.popleft();
            auto extracted_key = min_key + static_cast<int32_t>(item.data.second);
            RC_ASSERT(extracted_key == keys[extracted_count]);
            extracted_count++;
        }

        RC_ASSERT(extracted_count == num_items);
    });
}

TEST_CASE("Property-based test: BPQueue decrease_key reduces key correctly") {
    rc::check("decrease_key() reduces item's key by delta", []() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(0, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key + 5, min_key + 30));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        mywheel::Dllink<std::pair<int, uint32_t>> item;
        item.data = std::make_pair(42, 0);

        auto initial_key = static_cast<int32_t>(*rc::gen::inRange(min_key + 3, max_key));
        auto delta = static_cast<uint32_t>(*rc::gen::inRange(1, initial_key - min_key));

        bpq.appendleft(item, initial_key);
        bpq.decrease_key(item, delta);

        auto new_key = min_key + static_cast<int32_t>(item.data.second);
        RC_ASSERT(new_key == initial_key - static_cast<int32_t>(delta));
    });
}

TEST_CASE("Property-based test: BPQueue increase_key increases key correctly") {
    rc::check("increase_key() increases item's key by delta", []() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(0, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key + 5, min_key + 30));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        mywheel::Dllink<std::pair<int, uint32_t>> item;
        item.data = std::make_pair(42, 0);

        auto initial_key = static_cast<int32_t>(*rc::gen::inRange(min_key, max_key - 5));
        auto delta = static_cast<uint32_t>(*rc::gen::inRange(1, max_key - initial_key));

        bpq.appendleft(item, initial_key);
        bpq.increase_key(item, delta);

        auto new_key = min_key + static_cast<int32_t>(item.data.second);
        RC_ASSERT(new_key == initial_key + static_cast<int32_t>(delta));
    });
}

TEST_CASE("Property-based test: BPQueue modify_key handles both increase and decrease") {
    rc::check("modify_key() correctly increases or decreases key based on delta sign", []() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(0, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key + 5, min_key + 30));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        mywheel::Dllink<std::pair<int, uint32_t>> item;
        item.data = std::make_pair(42, 0);

        auto initial_key = static_cast<int32_t>(*rc::gen::inRange(min_key + 2, max_key - 2));
        auto delta = static_cast<int32_t>(*rc::gen::inRange(-3, 3));

        bpq.appendleft(item, initial_key);
        bpq.modify_key(item, delta);

        auto new_key = min_key + static_cast<int32_t>(item.data.second);
        auto expected_key = initial_key + delta;
        RC_ASSERT(new_key == expected_key);
    });
}

// ============================================================================
// Dllist Property-Based Tests
// ============================================================================

TEST_CASE("Property-based test: Dllist append and pop consistency") {
    rc::check("After appending n items, popping returns them in correct order", []() {
        mywheel::Dllist<int> list;
        std::vector<int> values;

        auto num_items = static_cast<size_t>(*rc::gen::inRange(1, 100));
        for (size_t i = 0; i < num_items; ++i) {
            auto val = *rc::gen::inRange(-1000, 1000);
            values.push_back(val);
            list.append(val);
        }

        std::vector<int> popped;
        while (!list.is_empty()) {
            popped.push_back(list.pop());
        }

        RC_ASSERT(popped == values);
    });
}

TEST_CASE("Property-based test: Dllist size reflects number of items") {
    rc::check("Size increases with append() and decreases with pop()", []() {
        mywheel::Dllist<int> list;

        auto append_count = static_cast<size_t>(*rc::gen::inRange(1, 50));
        for (size_t i = 0; i < append_count; ++i) {
            auto val = *rc::gen::inRange(-1000, 1000);
            list.append(val);
            RC_ASSERT(list.size() == static_cast<size_t>(i + 1));
        }

        auto pop_count = static_cast<size_t>(*rc::gen::inRange(1, append_count));
        for (size_t i = 0; i < pop_count; ++i) {
            list.pop();
            RC_ASSERT(list.size() == append_count - i - 1);
        }
    });
}

TEST_CASE("Property-based test: Dllist appendleft and popleft consistency") {
    rc::check("After appendleft() n items, popleft() returns them in LIFO order", []() {
        mywheel::Dllist<int> list;
        std::vector<int> values;

        auto num_items = static_cast<size_t>(*rc::gen::inRange(1, 100));
        for (size_t i = 0; i < num_items; ++i) {
            auto val = *rc::gen::inRange(-1000, 1000);
            values.push_back(val);
            list.appendleft(val);
        }

        std::vector<int> popped;
        while (!list.is_empty()) {
            popped.push_back(list.popleft());
        }

        std::reverse(values.begin(), values.end());
        RC_ASSERT(popped == values);
    });
}

// ============================================================================
// Robin Property-Based Tests
// ============================================================================

TEST_CASE("Property-based test: Robin cycle contains all parts") {
    rc::check("Robin cycle iterates through all parts except excluded one", []() {
        auto num_parts = static_cast<int>(*rc::gen::inRange(2, 20));
        auto exclude_part = static_cast<int>(*rc::gen::inRange(0, num_parts));

        fun::Robin<int> robin(num_parts);
        auto iterable = robin.exclude(exclude_part);

        std::vector<int> visited;
        for (const auto& part : iterable) {
            visited.push_back(part);
        }

        RC_ASSERT(visited.size() == static_cast<size_t>(num_parts - 1));
        RC_ASSERT(std::find(visited.begin(), visited.end(), exclude_part) == visited.end());

        for (int i = 0; i < num_parts; ++i) {
            if (i != exclude_part) {
                RC_ASSERT(std::find(visited.begin(), visited.end(), i) != visited.end());
            }
        }
    });
}

TEST_CASE("Property-based test: Robin exclude returns correct count") {
    rc::check("exclude() returns iterator for exactly n-1 parts", []() {
        auto num_parts = static_cast<int>(*rc::gen::inRange(2, 20));
        auto exclude_part = static_cast<int>(*rc::gen::inRange(0, num_parts));

        fun::Robin<int> robin(num_parts);
        auto iterable = robin.exclude(exclude_part);

        size_t count = 0;
        for ([[maybe_unused]] const auto& part : iterable) {
            count++;
        }

        RC_ASSERT(count == static_cast<size_t>(num_parts - 1));
    });
}

TEST_CASE("Property-based test: Robin cycle is circular") {
    rc::check("Multiple iterations produce the same sequence", []() {
        auto num_parts = static_cast<int>(*rc::gen::inRange(3, 20));
        auto exclude_part = static_cast<int>(*rc::gen::inRange(0, num_parts));

        fun::Robin<int> robin(num_parts);
        auto iterable1 = robin.exclude(exclude_part);
        auto iterable2 = robin.exclude(exclude_part);

        std::vector<int> seq1, seq2;
        for (const auto& part : iterable1) {
            seq1.push_back(part);
        }
        for (const auto& part : iterable2) {
            seq2.push_back(part);
        }

        RC_ASSERT(seq1 == seq2);
    });
}

// ============================================================================
// BPQueue Iterator Property-Based Tests
// ============================================================================

TEST_CASE("Property-based test: BPQueue iterator visits all items") {
    rc::check("Iterator visits all items in descending key order", []() {
        auto min_key = static_cast<int32_t>(*rc::gen::inRange(0, 10));
        auto max_key = static_cast<int32_t>(*rc::gen::inRange(min_key, min_key + 20));
        mywheel::BPQueue<int, int32_t> bpq(min_key, max_key);

        std::vector<mywheel::Dllink<std::pair<int, uint32_t>>> items;
        auto num_items = static_cast<size_t>(*rc::gen::inRange(5, 50));

        for (size_t i = 0; i < num_items; ++i) {
            items.emplace_back();
            auto key = static_cast<int32_t>(*rc::gen::inRange(min_key, max_key + 1));
            items[i].data = std::make_pair(static_cast<int>(i), 0);
            bpq.appendleft(items[i], key);
        }

        std::vector<int32_t> iterated_keys;
        for (auto& item : bpq) {
            auto key = min_key + static_cast<int32_t>(item.data.second);
            iterated_keys.push_back(key);
        }

        // Keys should be in descending order
        for (size_t i = 1; i < iterated_keys.size(); ++i) {
            RC_ASSERT(iterated_keys[i - 1] >= iterated_keys[i]);
        }

        RC_ASSERT(iterated_keys.size() == num_items);
    });
}

#endif