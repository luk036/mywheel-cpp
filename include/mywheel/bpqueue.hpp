#pragma once

#include <cassert>      // for assert
#include <cstdint>      // for int32_t
#include <type_traits>  // for make_unsigned_t, is_integral, integral_consta...
#include <utility>      // for pair
#include <vector>       // for vector, vector<>::value_type, vector<>::const...

#include "dllist.hpp"  // for Dllink, DllIterator

    // Forward declaration for begin() end()
    template <typename Tp, typename Int> class BpqIterator;

    /**
     * @brief Bounded Priority Queue with integer keys
     *
     * A highly efficient priority queue implementation that works with integer keys
     * within a specified bounded range [a..b]. The implementation uses an array of
     * doubly-linked lists (buckets), where each bucket contains all items with the
     * same priority/key value.
     *
     * Key Features:
     * - O(1) insertion and removal operations
     * - O(1) access to maximum priority element
     * - Memory efficient for small key ranges
     * - Non-owning node design for flexibility
     * - Sentinel element for boundary optimization
     *
     * The class does not own the queue nodes, allowing them to be shared between
     * this priority queue and other data structures (like doubly-linked lists).
     * This design is particularly useful in algorithms like Fiduccia-Mattheyses
     * where nodes move between different containers.
     *
     * The implementation includes a sentinel bucket to reduce boundary checking
     * overhead during key updates and operations.
     *
     * Time Complexity:
     * - Insert: O(1)
     * - Remove max: O(1)
     * - Update key: O(1)
     * - Check empty: O(1)
     *
     * Space Complexity: O(b-a+1) for the bucket array + O(n) for nodes
     *
     * ```cpp
     * Priority Queue (BPQueue) with integer keys [0..4]:
     *
     * Key 4: ┌─┐
     *        │7│ -> ...
     *        └─┘
     *
     * Key 3: ┌─┐
     *        │2│ -> │9│ -> ...
     *        └─┘
     *
     * Key 2: (empty)
     *
     * Key 1: ┌─┐
     *        │5│ -> │1│ -> │8│ -> ...
     *        └─┘
     *
     * Key 0: (empty)
     *
     * Sentinel: (dummy element for boundary checks)
     *
     * Max key: 4
     * ```
     *
     * @tparam Tp The type of payload data stored in each node
     * @tparam Int The integer type used for keys (default: int32_t)
     * @tparam Sequence The container type for buckets (default: vector of Dllist)
     */
    template <typename Tp, typename Int = int32_t,
              typename Sequence = std::vector<Dllist<std::pair<Tp, std::make_unsigned_t<Int>>>>>
    class BPQueue {
        using UInt = std::make_unsigned_t<Int>;

        friend BpqIterator<Tp, Int>;
        using Item = Dllink<std::pair<Tp, UInt>>;

        // static_assert(std::is_same<Item, typename _Sequence::value_type>::value,
        //               "value_type must be the same as the underlying container");

      public:
        using value_type = typename Sequence::value_type;
        using reference = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;
        using size_type = typename Sequence::size_type;
        using container_type = Sequence;

      private:
        Item sentinel{};  //!< Sentinel node for boundary optimization
        Sequence bucket;  //!< Array of doubly-linked lists, one for each key value
        UInt max{};       //!< Current maximum key value with non-empty bucket
        Int offset;       //!< Offset value (a - 1) for key translation
        UInt high;        //!< Maximum allowed key value (b - a + 1)

      public:
        /**
         * @brief Construct a bounded priority queue with specified key range
         *
         * Creates a BPQueue that can handle integer keys in the range [a, b].
         * The internal bucket array is sized to (b - a + 2) to include the sentinel.
         * The sentinel is automatically initialized to optimize boundary operations.
         *
         * @param[in] a The lower bound of allowed keys (inclusive)
         * @param[in] b The upper bound of allowed keys (inclusive)
         *
         * @pre a <= b
         * @pre Int must be an integral type
         *
         * @post The queue is empty and ready for operations
         */
        constexpr BPQueue(Int a, Int b)
            : bucket(static_cast<UInt>(b - a) + 2U),
              offset(a - 1),
              high(static_cast<UInt>(b - offset)) {
            assert(a <= b);
            static_assert(std::is_integral<Int>::value, "bucket's key must be an integer");
            bucket[0].appendleft(this->sentinel);  // sentinel
        }

        // Rule of Five: Explicitly control copy/move operations
        BPQueue(const BPQueue &) = delete;  // Copy constructor deleted - nodes are non-owning
        ~BPQueue() = default;
        constexpr auto operator=(const BPQueue &) -> BPQueue & = delete;  // Copy assignment deleted
        constexpr BPQueue(BPQueue &&) noexcept = default;  // Move constructor allowed
        constexpr auto operator=(BPQueue &&) noexcept
            -> BPQueue & = default;  // Move assignment allowed

        /**
         * @brief Check if the priority queue is empty
         *
         * Returns true if there are no elements in the queue. This is determined
         * by checking if the current maximum key is 0 (the sentinel position).
         *
         * @return true if empty, false otherwise
         */
        [[nodiscard]] constexpr auto is_empty() const noexcept -> bool { return this->max == 0U; }

        /**
         * @brief Set the internal key value of an item
         *
         * Sets the key stored in the item's data payload. The key is stored
         * internally as an unsigned value offset from the lower bound.
         *
         * @param[out] it Reference to the item whose key should be set
         * @param[in] gain The new key value for the item
         *
         * @pre gain must be within the queue's key bounds [a, b]
         */
        constexpr auto set_key(Item &it, Int gain) noexcept -> void {
            it.data.second = static_cast<UInt>(gain - this->offset);
        }

        /**
         * @brief Get the current maximum key value in the queue
         *
         * Returns the highest key value that has non-empty items in the queue.
         * If the queue is empty, this returns the offset value (a - 1).
         *
         * @return Int The maximum key value, or offset if empty
         */
        [[nodiscard]] constexpr auto get_max() const noexcept -> Int {
            return this->offset + Int(this->max);
        }

        /**
         * @brief Clear all elements from the priority queue
         *
         * Removes all items from all buckets and resets the maximum key to 0.
         * The queue becomes empty after this operation but retains its key bounds.
         * Note that nodes are not destroyed, only detached from the buckets.
         */
        constexpr auto clear() noexcept -> void {
            while (this->max > 0) {
                this->bucket[this->max].clear();
                this->max -= 1;
            }
        }

        /**
         * @brief Append item using its pre-set internal key
         *
         * Inserts an item into the queue using the key value already stored
         * in the item's data payload. This is more efficient than appendleft()
         * when the key is already known and stored in the item.
         *
         * @param[in,out] it Reference to the item to insert
         *
         * @pre The item must have a valid internal key set via set_key()
         * @pre The internal key must be > offset
         */
        constexpr auto appendleft_direct(Item &it) noexcept -> void {
            assert(static_cast<Int>(it.data.second) > this->offset);
            this->appendleft(it, Int(it.data.second));
        }

        /**
         * @brief Append item to the front of its bucket with specified key
         *
         * Inserts an item into the bucket corresponding to key `k`, placing it
         * at the front of the bucket's doubly-linked list. The item's internal
         * key is automatically set and the max key is updated if necessary.
         *
         * @param[in,out] it Reference to the item to insert
         * @param[in] k The key value for the item
         *
         * @pre k must be > offset (within valid key range)
         * @post Item is inserted at front of bucket for key k
         * @post max key is updated if k is greater than current max
         */
        constexpr auto appendleft(Item &it, Int k) noexcept -> void {
            assert(k > this->offset);
            it.data.second = UInt(k - this->offset);
            if (this->max < it.data.second) {
                this->max = it.data.second;
            }
            this->bucket[it.data.second].appendleft(it);
        }

        /**
         * @brief Append item to the back of its bucket with specified key
         *
         * Inserts an item into the bucket corresponding to key `k`, placing it
         * at the back of the bucket's doubly-linked list. The item's internal
         * key is automatically set and the max key is updated if necessary.
         *
         * @param[in,out] it Reference to the item to insert
         * @param[in] k The key value for the item
         *
         * @pre k must be > offset (within valid key range)
         * @post Item is inserted at back of bucket for key k
         * @post max key is updated if k is greater than current max
         */
        constexpr auto append(Item &it, Int k) noexcept -> void {
            assert(k > this->offset);
            it.data.second = UInt(k - this->offset);
            if (this->max < it.data.second) {
                this->max = it.data.second;
            }
            this->bucket[it.data.second].append(it);
        }

        /**
         * @brief Remove and return the item with the highest key
         *
         * Removes the item at the front of the bucket with the current maximum
         * key value. If that bucket becomes empty, automatically updates the max
         * key to point to the next non-empty bucket.
         *
         * @return Dllink& Reference to the removed item
         *
         * @pre The queue must not be empty
         * @post The returned item is no longer in any bucket
         * @post max key is updated if the highest bucket becomes empty
         */
        constexpr auto popleft() noexcept -> Item & {
            auto &res = this->bucket[this->max].popleft();
            while (this->bucket[this->max].is_empty()) {
                this->max -= 1;
            }
            return res;
        }

        /**
         * @brief Decrease the key of an item by the specified delta
         *
         * Removes the item from its current bucket, decreases its key by delta,
         * and inserts it at the back of the new bucket. Items with the same key
         * are processed in FIFO order. This method is commonly used in algorithms
         * like Fiduccia-Mattheyses where gain values decrease over time.
         *
         * @param[in,out] it Reference to the item whose key should be decreased
         * @param[in] delta The amount to decrease the key by
         *
         * @pre delta > 0
         * @pre it must be currently in the queue
         * @pre New key must be within valid range
         *
         * @note The order of items with the same key is not preserved (FIFO behavior)
         */
        constexpr auto decrease_key(Item &it, UInt delta) noexcept -> void {
            // this->bucket[it.data.second].detach(it)
            it.detach();
            it.data.second -= delta;
            assert(it.data.second > 0);
            assert(it.data.second <= this->high);
            this->bucket[it.data.second].append(it);  // FIFO
            if (this->max < it.data.second) {
                this->max = it.data.second;
                return;
            }
            while (this->bucket[this->max].is_empty()) {
                this->max -= 1;
            }
        }

        /**
         * @brief Increase the key of an item by the specified delta
         *
         * Removes the item from its current bucket, increases its key by delta,
         * and inserts it at the front of the new bucket. Items with the same key
         * are processed in LIFO order. This method is useful when gain values
         * need to be increased during algorithm execution.
         *
         * @param[in,out] it Reference to the item whose key should be increased
         * @param[in] delta The amount to increase the key by
         *
         * @pre delta > 0
         * @pre it must be currently in the queue
         * @pre New key must be within valid range
         *
         * @note The order of items with the same key is not preserved (LIFO behavior)
         */
        constexpr auto increase_key(Item &it, UInt delta) noexcept -> void {
            // this->bucket[it.data.second].detach(it)
            it.detach();
            it.data.second += delta;
            assert(it.data.second > 0);
            assert(it.data.second <= this->high);
            this->bucket[it.data.second].appendleft(it);  // LIFO
            if (this->max < it.data.second) {
                this->max = it.data.second;
            }
        }

        /**
         * @brief Modify the key of an item by the specified delta
         *
         * A unified method that can increase or decrease an item's key based on
         * the sign of delta. This is the preferred method for key updates as
         * it handles both directions and respects item locking. If the item
         * is locked, the operation is ignored.
         *
         * @param[in,out] it Reference to the item whose key should be modified
         * @param[in] delta The amount to change the key (positive for increase, negative for
         * decrease)
         *
         * @pre it must be currently in the queue
         * @pre New key must be within valid range
         *
         * @note If the item is locked, no modification is performed
         */
        constexpr auto modify_key(Item &it, Int delta) noexcept -> void {
            if (it.is_locked()) {
                return;
            }
            if (delta > 0) {
                this->increase_key(it, UInt(delta));
            } else if (delta < 0) {
                this->decrease_key(it, UInt(-delta));
            }
        }

        /**
         * @brief Remove an item from the queue without returning it
         *
         * Detaches the specified item from its bucket and updates the max key
         * if necessary. This is useful when you need to remove an item but
         * don't need to process it immediately.
         *
         * @param[in,out] it Reference to the item to detach
         *
         * @pre it must be currently in the queue
         * @post The item is no longer in any bucket
         * @post max key is updated if the highest bucket becomes empty
         */
        constexpr auto detach(Item &it) noexcept -> void {
            // this->bucket[it.data.second].detach(it)
            it.detach();
            while (this->bucket[this->max].is_empty()) {
                this->max -= 1;
            }
        }

        /**
         * @brief Get iterator to the beginning of the queue
         *
         * Returns an iterator that starts from the current maximum key
         * and traverses all items in descending key order. This provides
         * access to items from highest to lowest priority.
         *
         * @return BpqIterator iterator to beginning (highest priority)
         */
        constexpr auto begin() -> BpqIterator<Tp, Int>;

        /**
         * @brief Get iterator to the end of the queue
         *
         * Returns an iterator that represents the termination condition
         * for iteration. This iterator does not point to a valid item
         * and should only be used for comparison.
         *
         * @return BpqIterator iterator to end (termination condition)
         */
        constexpr auto end() -> BpqIterator<Tp, Int>;
    };

    /**
     * @brief Iterator for traversing BPQueue in descending key order
     *
     * The BpqIterator provides forward iteration through all items in the
     * priority queue, starting from the highest key value and moving to
     * lower key values. Within each bucket, items are traversed in the
     * order they appear in the doubly-linked list.
     *
     * Important: Modifying the queue (detaching items) during iteration
     * may invalidate the iterator because it maintains a copy of the
     * current key value rather than a reference to the queue state.
     *
     * The iterator follows STL forward iterator concepts and can be used
     * with range-based for loops and STL algorithms.
     */
    template <typename Tp, typename Int = int32_t> class BpqIterator {
        using UInt = std::make_unsigned_t<Int>;

        // using value_type = Tp;
        // using key_type = Int;
        using Item = Dllink<std::pair<Tp, UInt>>;

      private:
        BPQueue<Tp, Int> &bpq;  //!< Reference to the priority queue being iterated
        UInt curkey;            //!< Current key value being processed
        DllIterator<std::pair<Tp, UInt>> curitem;  //!< Iterator within the current bucket's list

        /**
         * @brief Get reference to the current bucket's sentinel node
         *
         * Returns a reference to the sentinel node of the bucket corresponding
         * to the current key value. This is used internally for list operations.
         *
         * @return Item& Reference to the current bucket's sentinel node
         */
        constexpr auto curlist() -> Item & { return this->bpq.bucket[this->curkey]; }

      public:
        /**
         * @brief Construct a BpqIterator for a specific key in a queue
         *
         * Creates an iterator positioned at the specified key value in the
         * given priority queue. The iterator will start traversing from the
         * beginning of the bucket corresponding to this key.
         *
         * @param[in] bpq Reference to the priority queue to iterate over
         * @param[in] curkey The initial key value to start from
         */
        constexpr BpqIterator(BPQueue<Tp, Int> &bpq, UInt curkey)
            : bpq{bpq}, curkey{curkey}, curitem{bpq.bucket[curkey].begin()} {}

        /**
         * @brief Move to the next item
         *
         * @return BpqIterator& reference to self
         */
        constexpr auto operator++() -> BpqIterator & {
            ++this->curitem;
            while (this->curitem == this->curlist().end()) {
                do {
                    this->curkey -= 1;
                } while (this->curlist().is_empty());
                this->curitem = this->curlist().begin();
            }
            return *this;
        }

        /**
         * @brief Get the reference of the current item
         *
         * @return Item& reference to current item
         */
        constexpr auto operator*() -> Item & { return *this->curitem; }

        /**
         * @brief eq operator
         *
         * @param[in] lhs left hand side iterator
         * @param[in] rhs right hand side iterator
         * @return true if equal, false otherwise
         */
        friend constexpr auto operator==(const BpqIterator &lhs, const BpqIterator &rhs) -> bool {
            return lhs.curitem == rhs.curitem;
        }

        /**
         * @brief neq operator
         *
         * @param[in] lhs left hand side iterator
         * @param[in] rhs right hand side iterator
         * @return true if not equal, false otherwise
         */
        friend constexpr auto operator!=(const BpqIterator &lhs, const BpqIterator &rhs) -> bool {
            return !(lhs == rhs);
        }
    };

    /**
     * @brief Begin iterator for BPQueue
     *
     * @return BpqIterator iterator to beginning
     */
    template <typename Tp, typename Int, class Sequence>
    inline constexpr auto BPQueue<Tp, Int, Sequence>::begin() -> BpqIterator<Tp, Int> {
        return {*this, this->max};
    }

    /**
     * @brief End iterator for BPQueue
     *
     * @return BpqIterator iterator to end
     */
    template <typename Tp, typename Int, class Sequence>
    inline constexpr auto BPQueue<Tp, Int, Sequence>::end() -> BpqIterator<Tp, Int> {
        return {*this, 0};
    }
