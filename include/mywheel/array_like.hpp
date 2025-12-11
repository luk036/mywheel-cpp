#pragma once

// #include <any>
#include <cstddef>
#include <stdexcept>
#include <vector>

/**
 * @brief The `RepeatArray` class is a template class that represents an array that repeats its elements.
 * It has a constructor that takes a value and a size as parameters and initializes all elements of the
 * array with the given value.
 *
 * ```
 *    0   1   2   3   4
 * +---+---+---+---+---+
 * | 5 | 5 | 5 | 5 | 5 |  <-- All elements have the same value
 * +---+---+---+---+---+
 * ```
 */
template <typename T> class RepeatArray {
  private:
    T _value;
    size_t _size;

  public:
    /**
     * Constructor for RepeatArray class.
     *
     * Initializes a RepeatArray with the given value repeated
     * size number of times.
     *
     * @param[in] value Value to initialize array elements with.
     * @param[in] size Number of elements in array.
     */
    RepeatArray(const T& value, size_t size) : _value(value), _size(size) {}

    /**
     * Overloads the subscript operator [] to return the value stored in the RepeatArray.
     *
     * @return The value stored in the RepeatArray.
     */
    const T& operator[](size_t /* index */) const noexcept { return this->_value; }

    /**
     * Returns the number of elements in the RepeatArray.
     *
     * @return the value of the variable "size" as a size_t data type.
     */
    size_t size() const noexcept { return this->_size; }

    /**
     * Iterator class for RepeatArray, allowing iteration over the elements of the array.
     *
     * The `begin()` function returns an iterator pointing to the beginning of the
     * RepeatArray, while the `end()` function returns an iterator pointing to the end of the
     * RepeatArray.
     */
    class Iterator {
      private:
        const RepeatArray<T>& _array;
        size_t _count;

      public:
        /**
         * Constructor for the Iterator class.
         *
         * Initializes an Iterator instance with a reference to the RepeatArray it will
         * iterate over, and a count representing the current position in the iteration.
         *
         * @param[in] array Reference to the RepeatArray to iterate over.
         * @param[in] count Current position in the iteration.
         */
        Iterator(const RepeatArray<T>& array, size_t count) : _array(array), _count(count) {}

        /**
         * Compares two Iterator instances for inequality.
         *
         * This operator overload checks if the current count of this Iterator
         * is not equal to the count of the other Iterator passed in.
         *
         * @param[in] other Iterator to compare to.
         * @return True if the counts are not equal, false otherwise.
         */
        bool operator!=(const Iterator& other) const noexcept { return this->_count != other._count; }

        /**
         * Returns the value stored in the underlying RepeatArray.
         *
         * This overloads the dereference operator for the Iterator class.
         * It returns the value stored in the RepeatArray that this Iterator
         * is iterating over.
         *
         * @return The value stored in the underlying RepeatArray.
         */
        const T& operator*() const noexcept { return this->_array._value; }

        /**
         * Pre-increment operator overload for Iterator class.
         *
         * Increments the internal count variable and returns a reference to self.
         *
         * @return The iterator object itself is being returned.
         */
        Iterator& operator++() noexcept {
            this->_count++;
            return *this;
        }
    };

    /**
     * Returns an iterator pointing to the first element of the container.
     *
     * This begins iteration at the start of the underlying container.
     *
     * @return Iterator object pointing to the first element.
     */
    Iterator begin() const { return Iterator(*this, 0); }

    /**
     * Returns an iterator pointing to the past-the-end element of the container.
     *
     * This ends iteration at the end of the underlying container.
     * Trying to dereference the returned iterator results in undefined behavior.
     *
     * @return Iterator pointing past the last element.
     */
    Iterator end() const { return Iterator(*this, this->_size); }
};

/**
 * @brief Shift Array
 *
 * The `ShiftArray` class is a template class that extends a given container
 * type. It allows accessing elements of the container using shifted indices.
 * The shift value is set using the `set_start` method, and the shifted indices
 * are calculated by subtracting the shift value from the original index. The
 * class provides `operator[]` overloads to access elements using shifted
 * indices.
 *
 * ```
 * Original indices:  [0] [1] [2] [3] [4] [5]
 * Array content:     [A] [B] [C] [D] [E] [F]
 *
 * After set_start(2):
 *
 * Accessible range:        [0] [1] [2] [3]  <-- These map to [C] [D] [E] [F]
 * Shifted indices:         [2] [3] [4] [5]  <-- Internal indices
 * ```
 *
 * @tparam T
 */
template <typename T> class ShiftArray {
  private:
    size_t _start = 0;
    std::vector<T> _lst;

  public:
    ShiftArray() : _lst{} {}

    /**
     * The ShiftArray function takes a vector as input and initializes the start variable to 0 and
     * the lst variable to the input vector.
     *
     * @param[in] lst The parameter "lst" is a vector of type T, which is the type of elements
     * stored in the vector.
     */
    explicit ShiftArray(const std::vector<T>& lst) : _lst(lst) {}

    explicit ShiftArray(std::vector<T>&& lst) noexcept : _lst(std::move(lst)) {}

    void set_start(size_t start) {
        if (start > this->_lst.size()) {
            throw std::out_of_range("start index is out of range");
        }
        this->_start = start;
    }

    /**
     * The above function is an overloaded operator[] that returns a constant reference to an
     * element in a list.
     *
     * @return The operator[] is returning a constant reference to an element in the list.
     */
    const T& operator[](size_t key) const { return this->_lst[key - this->_start]; }

    /**
     * The function overloads the subscript operator to access elements in a list-like container.
     *
     * @return The T& operator[] function returns a reference to an element in the lst vector, which
     * is accessed using the key parameter.
     */
    T& operator[](size_t key) { return this->_lst[key - this->_start]; }

    /**
     * Iterator class for ShiftArray, allowing iteration over elements in the array.
     */
    class Iterator {
      private:
        const ShiftArray<T>& _array;
        size_t _count;

      public:
        /**
         * The Iterator constructor takes a ShiftArray object and a count as parameters.
         *
         * @param[in] array The "array" parameter is of type "ShiftArray<T>", which is a template
         * class representing an array that can be shifted (rotated) to the left or right. It is
         * passed by reference to the constructor of the "Iterator" class.
         * @param[in] count The count parameter is of type size_t and represents the number of
         * elements in the ShiftArray that the Iterator will iterate over.
         */
        Iterator(const ShiftArray<T>& array, size_t count) : _array(array), _count(count) {}

        /**
         * The function checks if the count of two iterators are not equal.
         *
         * @param[in] other The "other" parameter is a reference to another Iterator object.
         *
         * @return a boolean value.
         */
        bool operator!=(const Iterator& other) const noexcept { return this->_count != other._count; }

        /**
         * The function returns the value at the current position in an array.
         *
         * @return The code is returning the element at the index `array.start + count` in the
         * `array.lst` array.
         */
        const T& operator*() const { return this->_array._lst[this->_array._start + this->_count]; }

        /**
         * The above function overloads the pre-increment operator for an Iterator class,
         * incrementing the count variable and returning a reference to the updated object.
         *
         * @return The iterator object itself is being returned.
         */
        Iterator& operator++() noexcept {
            this->_count++;
            return *this;
        }
    };

    /**
     * The function returns an iterator pointing to the beginning of a container.
     *
     * @return an iterator object.
     */
    Iterator begin() const { return Iterator(*this, 0); }

    /**
     * The end() function returns an iterator pointing to the end of a list.
     *
     * @return The end iterator of the container.
     */
    Iterator end() const {
        if (this->_start > this->_lst.size()) {
            return Iterator(*this, 0);
        }
        return Iterator(*this, this->_lst.size() - this->_start);
    }

    /**
     * The function returns the size of a list, excluding a specified starting index.
     *
     * @return the size of the list (lst) minus the starting index (start).
     */
    size_t size() const noexcept { return this->_lst.size() - this->_start; }
};
