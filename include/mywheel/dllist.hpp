#pragma once

#include <utility>  // import std::move()

#include "dllink.hpp"  // for Dllink

// Forward declaration for begin() end()
template <typename T> class DllIterator;

/**
 * @brief Doubly-linked list with sentinel head node for efficient operations
 *
 * The Dllist class implements a doubly-linked list using a sentinel head node
 * pattern that eliminates edge cases and provides O(1) insertion and deletion
 * operations. The design is optimized for algorithms where nodes frequently
 * move between different lists, such as the Fiduccia-Mattheyses algorithm.
 *
 * Key Features:
 * - Sentinel head eliminates special cases for empty lists
 * - O(1) insertion, deletion, and access operations
 * - No length tracking for maximum efficiency
 * - Non-owning design for flexible node management
 * - Packed structure for memory efficiency
 * - Iterator support for STL compatibility
 *
 * The sentinel approach means the list always has at least one node (the head),
 * which simplifies all operations and eliminates the need for null pointer checks.
 *
 * Time Complexity:
 * - Insert (front/back): O(1)
 * - Remove (front/back): O(1)
 * - Check empty: O(1)
 * - Iterator operations: O(1)
 *
 * Space Complexity: O(1) for list structure + O(n) for nodes
 *
 * ```cpp
 * Doubly Linked List with Sentinel Head:
 *
 *     ┌─────────────────┐
 *     │    Dllist       │
 *     │  ┌───────────┐  │
 *     └─▶│   head    │  │
 *        │ (sentinel)│  │
 *        └───────────┘  │
 *          ▲     ▲      │
 *          │     │      │
 *     ┌─────┐   ┌─────┐ │
 *     │prev │───│next │─┘
 *     └─────┘   └─────┘
 *       │         │
 *       │    ┌────▼──┬─────────┐
 *       │    │       │         │
 *       │    │ ┌─────▼───┐     │
 *       │    │ │  Node   │     │
 *       │    │ │         │     │
 *       │    │ └─────────┘     │
 *       │    │     │           │
 *       │    │     ▼           │
 *       │    │ ┌─────────┐     │
 *       │    │ │  Node   │     │
 *       │    │ │         │     │
 *       │    │ └─────────┘     │
 *       │    │     │           │
 *       │    │     ▼           │
 *       │    │ ┌─────────┐     │
 *       │    │ │  Node   │     │
 *       │    │ │(tail)   │     │
 *       │    │ └─────────┘     │
 *       │    │     │           │
 *       └────┴─────┼───────────┘
 *                   ▼
 *              ┌─────────┐
 *              │  Node   │
 *              │(head)   │
 *              └─────────┘
 *
 * The sentinel head simplifies operations by eliminating special cases
 * for empty lists and boundary nodes.
 * ```
 *
 * @tparam T The type of data stored in the nodes
 */
#pragma pack(push, 1)
template <typename T> class Dllist {
    friend DllIterator<T>;

  private:
    Dllink<T> head;

  public:
    /**
     * @brief Construct a new Dllist object
     *
     * @param[in] data the data
     */
    constexpr explicit Dllist(T data) noexcept : head{std::move(data)} {
        static_assert(sizeof(Dllist) <= 24, "keep this class small");
    }

    /**
     * @brief Copy construct a new Dllist object (deleted intentionally)
     *
     */
    constexpr Dllist() = default;
    ~Dllist() = default;
    Dllist(const Dllist &) = delete;                                // don't copy
    constexpr auto operator=(const Dllist &) -> Dllist & = delete;  // don't assign
    constexpr Dllist(Dllist &&) noexcept = default;
    constexpr auto operator=(Dllist &&) noexcept -> Dllist & = default;  // don't assign

    /**
     * @brief whether the list is empty
     *
     * @return true if empty, false otherwise
     */
    constexpr auto is_empty() const noexcept -> bool { return this->head.next == &this->head; }

    /**
     * @brief reset the list
     *
     */
    constexpr auto clear() noexcept -> void { this->head.next = this->head.prev = &this->head; }

    /**
     * @brief append the node to the front
     *
     * @param[in,out] node
     */
    constexpr auto appendleft(Dllink<T> &node) noexcept -> void { this->head.attach(node); }

    /**
     * @brief append the node to the back
     *
     * @param[in,out] node
     */
    constexpr auto append(Dllink<T> &node) noexcept -> void { this->head.prev->attach(node); }

    /**
     * @brief pop a node from the front
     *
     * @return Dllink& reference to popped node
     *
     * Precondition: list is not empty
     */
    constexpr auto popleft() noexcept -> Dllink<T> & {
        auto res = this->head.next;
        res->detach();
        return *res;
    }

    /**
     * @brief pop a node from the back
     *
     * @return Dllink& reference to popped node
     *
     * Precondition: list is not empty
     */
    constexpr auto pop() noexcept -> Dllink<T> & {
        auto res = this->head.prev;
        res->detach();
        return *res;
    }

    // For iterator

    /**
     * @brief Get iterator to beginning of list
     *
     * @return DllIterator iterator to beginning
     */
    constexpr auto begin() noexcept -> DllIterator<T>;

    /**
     * @brief Get iterator to end of list
     *
     * @return DllIterator iterator to end
     */
    constexpr auto end() noexcept -> DllIterator<T>;
};
#pragma pack(pop)

/**
 * @brief list iterator
 *
 * List iterator. Traverse the list from the first item. Usually it is
 * safe to attach/detach list items during the iterator is active.
 */
template <typename T> class DllIterator {
  private:
    Dllink<T> *cur; /**< pointer to the current item */

  public:
    /**
     * @brief Construct a new dll iterator object
     *
     * @param[in] cur
     */
    constexpr explicit DllIterator(Dllink<T> *cur) noexcept : cur{cur} {}

    /**
     * @brief move to the next item
     *
     * @return DllIterator& reference to self
     */
    constexpr auto operator++() noexcept -> DllIterator & {
        this->cur = this->cur->next;
        return *this;
    }

    /**
     * @brief get the reference of the current item
     *
     * @return Dllink& reference to current item
     */
    constexpr auto operator*() noexcept -> Dllink<T> & { return *this->cur; }

    /**
     * @brief eq operator
     *
     * @param[in] lhs left hand side iterator
     * @param[in] rhs right hand side iterator
     * @return true if equal, false otherwise
     */
    friend auto operator==(const DllIterator &lhs, const DllIterator &rhs) noexcept -> bool {
        return lhs.cur == rhs.cur;
    }

    /**
     * @brief neq operator
     *
     * @param[in] lhs left hand side iterator
     * @param[in] rhs right hand side iterator
     * @return true if not equal, false otherwise
     */
    friend auto operator!=(const DllIterator &lhs, const DllIterator &rhs) noexcept -> bool {
        return !(lhs == rhs);
    }
};

/**
 * @brief Get iterator to beginning of list
 *
 * @return DllIterator iterator to beginning
 */
template <typename T> constexpr auto Dllist<T>::begin() noexcept -> DllIterator<T> {
    return DllIterator<T>{this->head.next};
}

/**
 * @brief Get iterator to end of list
 *
 * @return DllIterator iterator to end
 */
template <typename T> constexpr auto Dllist<T>::end() noexcept -> DllIterator<T> {
    return DllIterator<T>{&this->head};
}
