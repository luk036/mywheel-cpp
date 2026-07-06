#pragma once

/** @file robin.hpp
 *  @brief Round-robin cycle implementation with index-based circular addressing.
 *
 *  Uses modular arithmetic instead of linked-list pointers, saving 8 bytes per node
 *  and avoiding pointer invalidation on vector reallocation.
 */

#include <cstddef>
#include <vector>

namespace fun {

    namespace detail {

        /**
         * @brief Iterator for traversing elements in a Robin cycle.
         * @tparam T Type of the key/value stored in nodes.
         */
        template <typename T> struct RobinIterator {
            const std::vector<T>* cycle;  //!< Pointer to the cycle vector
            std::size_t idx;              //!< Current index in the cycle
            std::size_t start_idx;        //!< Index where iteration ends (exclusive)

            /**
             * @brief Inequality comparison
             */
            auto operator!=(const RobinIterator& other) const -> bool { return idx != other.idx; }

            /**
             * @brief Equality comparison
             */
            auto operator==(const RobinIterator& other) const -> bool { return idx == other.idx; }

            /**
             * @brief Pre-increment — advance to next index (circular)
             */
            auto operator++() -> RobinIterator& {
                idx = (idx + 1) % cycle->size();
                return *this;
            }

            /**
             * @brief Dereference — return the key at current index
             */
            auto operator*() const -> const T& { return (*cycle)[idx]; }
        };

        /**
         * @brief Wrapper providing begin/end iteration excluding a specified part.
         * @tparam T Type of the key/value stored in nodes.
         */
        template <typename T> struct RobinIterableWrapper {
            const std::vector<T>* cycle;  //!< Pointer to the cycle vector
            std::size_t start_idx;        //!< Index of the excluded part

            /**
             * @brief Begin iterator — starts at the node after the excluded part
             */
            auto begin() const -> RobinIterator<T> {
                return RobinIterator<T>{cycle, (start_idx + 1) % cycle->size(), start_idx};
            }

            /**
             * @brief End iterator — points to the excluded part
             */
            auto end() const -> RobinIterator<T> {
                return RobinIterator<T>{cycle, start_idx, start_idx};
            }
        };
    }  // namespace detail

    /**
     * @brief Round Robin
     *
     * Cycles through a sequence of elements in a circular manner using
     * index-based addressing (no linked-list pointers). Each part is
     * assigned a unique key. The `exclude` method returns an iterable
     * wrapper that excludes a specified part from the cycle.
     *
     * Memory: O(num_parts) for the key vector, no pointer overhead.
     *
     * @verbatim
     * Round Robin Cycle (index-based):
     *
     *   cycle vector: [0] [1] [2] [3] [4] [5]
     *
     *   Iteration order:
     *   exclude(2) -> 3 -> 4 -> 5 -> 0 -> 1  (modular arithmetic)
     *
     *   No linked-list pointers needed: next = (idx + 1) % size
     * @endverbatim
     *
     * @tparam T Integer type for keys
     */
    template <typename T> struct Robin {
        using Iterator = detail::RobinIterator<T>;
        using IterableWrapper = detail::RobinIterableWrapper<T>;

        std::vector<T> cycle;  //!< Vector storing the keys (no pointer overhead)

        /**
         * @brief Construct a Robin cycle with num_parts elements.
         * @param num_parts Number of parts in the cycle.
         */
        explicit Robin(T num_parts)
            : cycle(static_cast<typename std::vector<T>::size_type>(num_parts)) {
            for (auto idx = T(0); idx != num_parts; ++idx) {
                cycle[static_cast<std::size_t>(idx)] = idx;
            }
        }

        /**
         * @brief Return an iterable wrapper that excludes a specified part.
         * @param from_part The part to exclude from iteration.
         * @return IterableWrapper that iterates over all parts except from_part.
         */
        [[nodiscard]] auto exclude(T from_part) const noexcept -> IterableWrapper {
            return IterableWrapper{&cycle, static_cast<std::size_t>(from_part)};
        }
    };

}  // namespace fun
