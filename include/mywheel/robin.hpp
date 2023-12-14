#pragma once

#include <vector>

namespace fun {

    namespace detail {

        /**
         * The code snippet is defining a struct template called `RobinSlNode`. It represents a node
         * in a singly linked list used in the implementation of the `Robin` class.
         *
         * @tparam T
         */
        template <typename T> struct RobinSlNode {
            RobinSlNode *next;
            T key;
        };

        /**
         * The code snippet defines a struct template called `RobinIterator`. This struct is used in
         * the implementation of the `Robin` class in the `fun` namespace.
         *
         * @tparam T
         */
        template <typename T> struct RobinIterator {
            const RobinSlNode<T> *cur;

            /**
             * The function checks if the current iterator is not equal to another iterator.
             *
             * @param other The parameter "other" is of type RobinIterator, which is the type of the
             * object being compared to the current object.
             *
             * @return The operator is returning a boolean value. If the current iterator is not
             * equal to the other iterator, it will return true. Otherwise, it will return false.
             */
            auto operator!=(const RobinIterator &other) const -> bool { return cur != other.cur; }

            /**
             * The function checks if the current iterator is equal to another iterator.
             *
             * @param other The parameter "other" is of type RobinIterator, which is the type of the
             * object being compared to the current object.
             *
             * @return The operator is returning a boolean value.
             */
            auto operator==(const RobinIterator &other) const -> bool { return cur == other.cur; }

            /**
             * The function increments the iterator to the next element and returns a reference to
             * the updated iterator.
             *
             * @return The `operator++` function is returning a reference to a `RobinIterator`
             * object.
             */
            auto operator++() -> RobinIterator & {
                cur = cur->next;
                return *this;
            }

            /**
             * The above function is an overloaded operator* that returns a const reference to the
             * key of the current node.
             *
             * @return The code is returning a reference to a constant object of type T.
             */
            auto operator*() const -> const T & { return cur->key; }
        };

        /**
         * The code snippet is defining a struct template called `RobinIterableWrapper`. This struct
         * is used in the implementation of the `Robin` class in the `fun` namespace.
         *
         * @tparam T
         */
        template <typename T> struct RobinIterableWrapper {
            const detail::RobinSlNode<T> *node;

            /**
             * The begin() function returns a RobinIterator object pointing to the next node.
             *
             * @return a `RobinIterator<T>` object.
             */
            auto begin() const -> RobinIterator<T> { return RobinIterator<T>{node->next}; }

            /**
             * The function returns a RobinIterator object representing the end of a collection.
             *
             * @return a `RobinIterator<T>` object.
             */
            auto end() const -> RobinIterator<T> { return RobinIterator<T>{node}; }
        };
    }  // namespace detail

    /**
     * @brief Round Robin
     *
     * The `Robin` class is implementing a round-robin algorithm. It is used to
     * cycle through a sequence of elements in a circular manner. The constructor
     * initializes the cycle with a specified number of parts, and each part is
     * assigned a unique key. The `exclude` method returns an iterable wrapper that
     * excludes a specified part from the cycle.
     *
     * @tparam T
     */
    template <typename T> struct Robin {
        std::vector<detail::RobinSlNode<T>> cycle;

        /**
         * The Robin constructor initializes a cycle of objects with keys ranging from 0 to
         * num_parts-1.
         *
         * @param num_parts The parameter `num_parts` represents the number of parts or elements in
         * the Robin object.
         */
        explicit Robin(T num_parts) : cycle(num_parts) {
            auto *slptr = &this->cycle[num_parts - 1];
            auto k = T(0);
            for (auto &sl : this->cycle) {
                sl.key = k;
                slptr->next = &sl;
                slptr = slptr->next;
                ++k;
            }
        }

        /**
         * The `exclude` method in the `Robin` class returns an iterable wrapper that excludes a
         * specified part from the cycle.
         *
         * @param from_part The `from_part` parameter in the `exclude` method is the part of the
         * cycle that you want to exclude. It is of type `T`, which is the same type as the elements
         * in the cycle.
         *
         * @return The `exclude` method in the `Robin` class returns an iterable wrapper of type
         * `detail::RobinIterableWrapper<T>`.
         */
        auto exclude(T from_part) const -> detail::RobinIterableWrapper<T> {
            return detail::RobinIterableWrapper<T>{&this->cycle[from_part]};
        }
    };

}  // namespace fun
