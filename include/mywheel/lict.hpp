#pragma once

#include <vector>

#include <py2cpp/enumerate.hpp>
#include <py2cpp/range.hpp>

namespace py {

    /**
     * @brief Dict-like data structure by std::vector and Range
     *
     * The `Lict` class is a custom implementation of an unordered mapping with integer keys and
     * generic values, which adapts a vector to behave like a dictionary.
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
        py::Range<key_type> _rng;
        std::vector<T> _lst;

      public:
        /**
         * @brief Constructor for a dictionary-like adaptor for a vector.
         *
         * @param[in] lst The `lst` parameter is a vector. It is used to initialize the `self.lst`
         * attribute of the class
         */
        explicit Lict(std::vector<T> lst) : _rng{py::range(lst.size())}, _lst(std::move(lst)) {}

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
        const T &operator[](const key_type &key) const { return this->_lst.at(key); }

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
        T &operator[](const key_type &key) { return this->_lst[key]; }

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
        const T &at(const key_type &key) const { return this->_lst.at(key); }

        // void erase() { throw std::runtime_error("NotImplementedError"); }

        /**
         * @brief
         *
         * @return iterator
         */
        iterator begin() const { return this->_rng.begin(); }

        /**
         * @brief
         *
         * @return iterator
         */
        iterator end() const { return this->_rng.end(); }

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
        bool contains(const key_type &key) const { return this->_rng.contains(key); }

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
        size_t size() const { return this->_rng.size(); }

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
        const auto &values() const { return this->_lst; }

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
        auto &values() { return this->_lst; }

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
        auto items() { return py::enumerate(this->_lst); }
    };

}  // namespace py

// int main() {
//   Lict<int> a(std::vector<int>(8, 0));
//   for (int i : a) {
//     a[i] = i * i;
//   }
//   for (auto i : a) {
//     std::cout << i << ": " << a[i] << std::endl;
//   }
//   std::cout << a.__contains__(3) << std::endl;
//   return 0;
// }
