#pragma once

#include <py2cpp/enumerate.hpp>
#include <py2cpp/range.hpp>
#include <vector>

namespace py {

    /**
     * @brief Dict-like data structure by std::vector and Range
     *
     * The `Lict` class is a custom implementation of an unordered mapping with integer keys and
     * generic values, which adapts a vector to behave like a dictionary.
     *
     * ```cpp
     * Lict (List-Dictionary) Structure:
     *
     * Vector-based mapping with integer keys:
     *
     * Keys:   [0] [1] [2] [3] [4]
     *        ┌───┬───┬───┬───┬───┐
     * Values: │ A │ B │ C │ D │ E │
     *        └───┴───┴───┴───┴───┘
     *
     * Access: lict[2] → returns 'C'
     *         lict.contains(3) → true
     *         lict.size() → 5
     * ```
     *
     * @tparam T
     */
    template <typename T> class Lict {
      public:
        using key_type = size_t;
        using value_type = T;
        using iterator = py::Range<key_type>::iterator;
        using const_iterator = py::Range<key_type>::iterator;

      private:
        // py::Range<key_type> _rng;
        std::vector<T> _lst;

      public:
        /**
         * @brief Constructor for a dictionary-like adaptor for a vector.
         *
         * @param[in] lst The `lst` parameter is a vector. It is used to initialize the `self.lst`
         * attribute of the class
         */
        explicit Lict(std::vector<T> lst) : _lst(std::move(lst)) {}

        /**
         * @brief This function allows you to access an element in a Lict object by its index.
         *
         * @param[in] key The `key` parameter is of type `size_t` and it represents the index of the
         * element that you want to retrieve from the list
         *
         * @return the item at the specified index in the `lst` attribute.
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> a[2]
         *   3
         *
         */
        auto operator[](const key_type &key) const -> const T& { return this->_lst.at(key); }

        /**
         * @brief This function sets the value at a given index in a list-like object.
         *
         * @param[in] key The key parameter represents the index at which the new value should be
         * set in the list
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> a[2] = 7
         *   >>> a[2]
         *   7
         *
         */
        auto operator[](const key_type &key) -> T& { return this->_lst[key]; }

        /**
         * @brief This function allows you to access an element in a Lict object by its index.
         *
         * @param[in] key The `key` parameter is of type `size_t` and it represents the index of the
         * element that you want to retrieve from the list
         *
         * @return the item at the specified index in the `lst` attribute.
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> a.at(2)
         *   3
         *
         */
        auto at(const key_type &key) const -> const T& { return this->_lst.at(key); }

        /**
         * @brief Get iterator to beginning
         *
         * @return iterator to beginning
         */
        auto begin() const noexcept -> iterator {
            return py::range<key_type>(this->_lst.size()).begin();
        }

        /**
         * @brief Get iterator to end
         *
         * @return iterator to end
         */
        auto end() const noexcept -> iterator { return py::range<key_type>(this->_lst.size()).end(); }

        /**
         * @brief The `contains` function checks if a given value is present in the `rng` attribute
         * of the object.
         *
         * @param[in] key The `key` parameter represents the value that we want to check if it is
         * present in the `self.rng` attribute
         * @return The method is returning a boolean value, indicating whether the given value is
         * present in the `self.rng` attribute.
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> a.contains(2)
         *   true
         */
        [[nodiscard]] auto contains(const key_type &key) const noexcept -> bool {
            return key < this->_lst.size();
        }

        /**
         * @brief This function returns the length of the `rng` attribute of the object.
         *
         * @return The `size` function is returning the size of the `self.rng` attribute.
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> a.size()
         *   4
         */
        [[nodiscard]] auto size() const noexcept -> size_t { return this->_lst.size(); }

        /**
         * @brief The `values` function returns an iterator that yields the elements of the `lst`
         * attribute of the `Lict` object.
         *
         * @return The `values` method returns a const reference to the vector object.
         *
         * Examples:
         *   >>> const auto a = Lict({1, 4, 3, 6});
         *   >>> for (const auto &i : a.values()) {
         *   ...     fmt::print(i);
         *   ... }
         *   1
         *   4
         *   3
         *   6
         */
        auto values() const -> const auto& { return this->_lst; }

        /**
         * @brief The `values` function returns an iterator that yields the elements of the `lst`
         * attribute of the `Lict` object.
         *
         * @return The `values` method returns a reference to the vector object.
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> for (auto& i : a.values()) {
         *   ...     i += 1;
         *   ...     fmt::print(i);
         *   ... }
         *   2
         *   5
         *   4
         *   7
         */
        auto values() -> auto& { return this->_lst; }

        /**
         * @brief The function returns an enumeration of the items in the list.
         *
         * @return: The `items` method is returning an enumeration of the `lst` attribute.
         *
         * Examples:
         *   >>> auto a = Lict({1, 4, 3, 6});
         *   >>> for (auto& [key, value] : a.items()) {
         *   ...     fmt::print(key, value);
         *   ... }
         *   (0, 1)
         *   (1, 4)
         *   (2, 3)
         *   (3, 6)
         */
        auto items() const noexcept { return py::enumerate(this->_lst); }
    };

}  // namespace py
