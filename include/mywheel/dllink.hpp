#pragma once

#include <cassert>
#include <utility>  // for std::move()

// Forward declaration for begin() end()
template <typename T> class Dllist;
template <typename T> class DllIterator;

/**
 * @brief Doubly-linked list node with sentinel support
 *
 * The Dllink class represents a node in a doubly-linked list that can also
 * serve as a list head (sentinel node). This design enables extremely fast
 * insertion and deletion operations without special cases for empty lists
 * or boundary conditions.
 *
 * Key Features:
 * - Can function as both regular node and list head (sentinel)
 * - O(1) insertion and deletion operations
 * - No length tracking for maximum efficiency
 * - Non-owning design for flexible node management
 * - Packed structure for memory efficiency
 *
 * The sentinel-based approach eliminates edge cases in list operations,
 * making the implementation simpler and more robust. This is particularly
 * useful in algorithms like Fiduccia-Mattheyses where nodes frequently
 * move between different lists.
 *
 * Time Complexity:
 * - Insert: O(1)
 * - Delete: O(1)
 * - Attach/Detach: O(1)
 *
 * Space Complexity: O(1) per node
 *
 * ```cpp
 * Doubly Linked List Structure:
 * 
 *     ┌─────────────────────────────────────┐
 *     │              Head/Sentinel          │
 *     │  ┌─────┐     ┌─────┐     ┌─────┐   │
 *     └─▶│next │────▶│next │────▶│next │───┘
 *        │     │     │     │     │     │
 *        │prev│◀────│prev│◀────│prev│◀─┐
 *        └─────┘     └─────┘     └─────┘ │
 *          ▲         ▲   ▲       ▲   ▲   │
 *          │         │   │       │   │   │
 *          └─────────┼───┼───────┼───┼───┘
 *                    │   │       │   │
 *                 ┌─────┐ ┌─────┐ ┌─────┐
 *                 │Node1│ │Node2│ │Node3│
 *                 │Data │ │Data │ │Data │
 *                 └─────┘ └─────┘ └─────┘
 * 
 * Each node has 'next' and 'prev' pointers, with the head/sentinel
 * serving as a boundary marker to simplify insertion/deletion.
 * ```cpp
 *
 * @tparam T The type of data stored in the node
 */
#pragma pack(push, 1)
template <typename T> class Dllink {
    friend DllIterator<T>;
    friend Dllist<T>;
    friend DllIterator<T>;

  private:
    Dllink *next{this}; /**< Pointer to the next node in the list */
    Dllink *prev{this}; /**< Pointer to the previous node in the list */

  public:
    T data{}; /**< User data payload stored in the node */

    /**
     * @brief Construct a Dllink node with initial data
     *
     * Creates a new Dllink node and initializes it with the provided data.
     * The node starts in a self-contained state where next and prev point to itself.
     * A static assert ensures the class remains memory-efficient.
     *
     * @param[in] data The data to store in the node (moved)
     * 
     * @post The node is initialized and ready for list operations
     */
    constexpr explicit Dllink(T data) noexcept : data{std::move(data)} {
        static_assert(sizeof(Dllink) <= 24, "keep this class small");
    }

    /**
     * @brief Default constructor
     *
     * Creates a Dllink node with default-initialized data.
     * The node starts in a self-contained state.
     */
    constexpr Dllink() = default;
    
    /**
     * @brief Destructor
     *
     * Cleans up the node. Note that this does not remove the node
     * from any list it might be part of - detach() should be called
     * first if the node is in a list.
     */
    ~Dllink() = default;
    
    // Rule of Five: Explicitly control operations to prevent unintended copying
    Dllink(const Dllink &) = delete;                      // Copy constructor deleted
    auto operator=(const Dllink &) -> Dllink & = delete;  // Copy assignment deleted
    constexpr Dllink(Dllink &&) noexcept = default;      // Move constructor allowed
    constexpr auto operator=(Dllink &&) noexcept -> Dllink & = delete;  // Move assignment deleted

    /**
     * @brief Lock the node to prevent list operations
     *
     * Locks the node by setting its next pointer to itself.
     * A locked node cannot be inserted into any list and will
     * be ignored by list operations. This is useful for temporarily
     * removing nodes from consideration without actually detaching them.
     *
     * @post The node is locked and cannot be used in list operations
     */
    constexpr auto lock() noexcept -> void { this->next = this; }

    /**
     * @brief Check if the node is locked
     *
     * Returns true if the node is locked (next pointer points to itself).
     * Locked nodes cannot participate in list operations.
     *
     * @return true if locked, false otherwise
     */
    [[nodiscard]] constexpr auto is_locked() const noexcept -> bool { return this->next == this; }

    /**
     * @brief Remove the node from its current list
     *
     * Detaches the node from whatever list it's currently in by
     * linking its previous and next nodes together. The node itself
     * becomes self-contained (next and prev point to itself).
     *
     * @pre The node must not be locked
     * @pre The node must be part of a list
     * 
     * @post The node is removed from the list and self-contained
     */
    constexpr auto detach() noexcept -> void {
        assert(!this->is_locked());
        const auto n = this->next;
        const auto p = this->prev;
        p->next = n;
        n->prev = p;
    }

  private:
    /**
     * @brief Attach another node after this node
     *
     * Inserts the specified node immediately after this node in the list.
     * This is a private method used internally by list operations.
     * The node being attached should not already be in another list.
     *
     * @param[in,out] node Reference to the node to attach
     * 
     * @pre node should not be currently in a list
     * @post node is inserted after this node
     */
    constexpr auto attach(Dllink &node) noexcept -> void {
        node.next = this->next;
        this->next->prev = &node;
        this->next = &node;
        node.prev = this;
    }
};
#pragma pack(pop)
