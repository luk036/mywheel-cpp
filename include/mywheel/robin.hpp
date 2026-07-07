#pragma once

/** @file robin.hpp
 *  @brief Round-robin cycle implementation with circular singly-linked list.
 */

#include <cassert>
#include <cstddef>
#include <vector>

namespace fun {

    namespace detail {

        /**
         * @brief Node in a singly-linked cycle for round-robin iteration.
         * @tparam T Type of the key/value stored in the node.
         */
        template <typename T> struct RobinSlNode {
            RobinSlNode* next;  //!< Pointer to the next node in the cycle
            T key;              //!< The key/value stored in this node
        };

        /**
         * @brief Iterator for traversing elements in a Robin cycle.
         *
         * Matches the Python RobinIterator pattern: stores both the current
         * node (`curr`) and the excluded node (`stop`).  Iteration stops when
         * `curr` wraps around to `stop`.
         *
         * @tparam T Type of the key/value stored in nodes.
         */
        template <typename T> struct RobinIterator {
            const RobinSlNode<T>* curr;  //!< Current node in the iteration
            const RobinSlNode<T>* stop;  //!< Excluded node – iteration stops when curr reaches this

            /**
             * @brief Construct an iterator.
             * @param c Pointer to the starting node.
             * @param s Pointer to the excluded (stop) node.
             */
            RobinIterator(const RobinSlNode<T>* c,
                          const RobinSlNode<T>* s) noexcept
                : curr(c), stop(s) {}

            /**
             * @brief Inequality comparison (used by range-for to check termination).
             */
            auto operator!=(const RobinIterator& other) const noexcept -> bool {
                return curr != other.curr;
            }

            /**
             * @brief Equality comparison.
             */
            auto operator==(const RobinIterator& other) const noexcept -> bool {
                return curr == other.curr;
            }

            /**
             * @brief Advance to the next node (matches Python: curr = curr.next).
             */
            auto operator++() -> RobinIterator& {
                curr = curr->next;
                return *this;
            }

            /**
             * @brief Dereference – access the current node's key.
             */
            auto operator*() const noexcept -> const T& { return curr->key; }
        };

        /**
         * @brief Wrapper providing begin/end iteration excluding a specified part.
         *
         * This is required for C++ range-based for loops and mirrors the
         * Python pattern where `exclude()` returns an iterable.
         *
         * @tparam T Type of the key/value stored in nodes.
         */
        template <typename T> struct RobinIterableWrapper {
            const RobinSlNode<T>* curr_node;  //!< Pointer to the excluded node

            /**
             * @brief Return an iterator pointing to the first valid element
             *        (the node after the excluded one).
             */
            auto begin() const -> RobinIterator<T> {
                // Match Python: start by advancing past the excluded node,
                // stop is the excluded node itself.
                return RobinIterator<T>{curr_node->next, curr_node};
            }

            /**
             * @brief Return a past-the-end iterator (points to the excluded node).
             */
            auto end() const -> RobinIterator<T> {
                return RobinIterator<T>{curr_node, curr_node};
            }
        };
    }  // namespace detail

    /**
     * @brief Round Robin scheduler.
     *
     * Implements a round-robin cycle using a circular singly-linked list.
     * The `exclude()` method returns an iterable that visits every node
     * **except** the excluded one, starting from the node immediately
     * after the excluded node.
     *
     * @verbatim
     * Round Robin Cycle:
     *
     *    ┌─────┐
     *    │  0  │◄────────────────────────┐
     *    └──┬──┘                        │
     *       │                           │
     *       ▼                           │
     *    ┌─────┐                        │
     *    │  1  │◄────┐                 │
     *    └──┬──┘     │                 │
     *       │        │                 │
     *       ▼        │                 │
     *    ┌─────┐     │                 │
     *    │  2  │◄────┼─────────────────┤  <-- exclude(2) skips this
     *    └──┬──┘     │                 │
     *       │        │                 │
     *       ▼        │                 │
     *    ┌─────┐     │                 │
     *    │  3  │◄────┘                 │
     *    └──┬──┘                       │
     *       │                          │
     *       └──────────────────────────┘
     * @endverbatim
     *
     * @tparam T Type of the key stored in each node (typically uint8_t or int).
     */
    template <typename T> struct Robin {
        using SlNode = detail::RobinSlNode<T>;
        using IterableWrapper = detail::RobinIterableWrapper<T>;

        std::vector<SlNode> cycle;  //!< Vector storing the circular linked list nodes

        /**
         * @brief Construct a round-robin cycle with `num_parts` nodes.
         *
         * Creates nodes with sequential keys 0, 1, …, `num_parts`-1 and
         * links them in a circular singly-linked list.
         *
         * Two-phase construction matching the Python implementation:
         *   1. Create nodes with their keys
         *   2. Link them into a circle
         */
        explicit Robin(T num_parts) {
            using size_type = typename std::vector<SlNode>::size_type;
            auto n = static_cast<size_type>(num_parts);
            assert(n >= 2 && "Robin: num_parts must be at least 2");

            // Phase 1: create nodes (matches Python: list(SlNode(k) for k in range(num_parts)))
            cycle.reserve(n);
            for (auto i = size_type{0}; i < n; ++i) {
                cycle.push_back(SlNode{nullptr, static_cast<T>(i)});
            }

            // Phase 2: link in a circle (matches Python: prev.next = curr)
            auto* prev = &cycle.back();
            for (auto& node : cycle) {
                prev->next = &node;
                prev = &node;
            }
        }

        /**
         * @brief Return an iterable that visits every node **except** `from_part`.
         *
         * Iteration begins at the node after `from_part` and stops when it
         * would circle back to the excluded node.
         *
         * @param from_part The node to exclude from iteration.
         */
        [[nodiscard]] auto exclude(T from_part) const -> IterableWrapper {
            using size_type = typename std::vector<SlNode>::size_type;
            auto idx = static_cast<size_type>(from_part);
            assert(idx < cycle.size() && "Robin::exclude: from_part out of range");
            return IterableWrapper{&cycle[idx]};
        }
    };

}  // namespace fun
