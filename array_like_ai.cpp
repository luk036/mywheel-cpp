#include <iostream>
#include <vector>

/* The `RepeatArray` class is a template class that represents an array that repeats its elements.
It has a constructor that takes a value and a size as parameters and initializes all elements of the
array with the given value. */
template <typename T> class RepeatArray {
  private:
    T _value;
    size_t _size;

  public:
    /**
     * The RepeatArray function creates an array of a given size and initializes all elements with a
     * given value.
     *
     * @param value The value parameter is of type T, which means it can be any data type. It
     * represents the value that will be repeated in the array.
     * @param size The size parameter is of type size_t and represents the size of the array.
     */
    RepeatArray(T value, size_t size) : _value(value), _size(size) {}

    /**
     * The above function overloads the subscript operator to return the value at the specified
     * index.
     *
     * @param index The parameter "index" is of type "size_t", which is an unsigned integer type
     * used to represent the size of objects in memory. It is typically used to index elements in an
     * array or container. In this context, "index" represents the position of an element in the
     * container that the operator
     *
     * @return The value of the variable "value" is being returned.
     */
    T operator[](size_t /* index */) const { return this->_value; }

    /**
     * The size() function returns the size of a data structure.
     *
     * @return the value of the variable "size" as a size_t data type.
     */
    size_t size() const { return this->_size; }

    /**
     * The code defines an iterator class for a repeat array, allowing iteration over the elements
     * of the array.
     *
     * @return The `begin()` function returns an iterator pointing to the beginning of the
     * RepeatArray, while the `end()` function returns an iterator pointing to the end of the
     * RepeatArray.
     */
    class Iterator {
      private:
        const RepeatArray<T>& _array;
        size_t _count;

      public:
        /**
         * The Iterator constructor takes a RepeatArray object and a count as parameters.
         *
         * @param array The "array" parameter is of type "RepeatArray<T>", which is a template class
         * representing an array that repeats its elements. It is likely that this class has a
         * member function or operator that allows accessing elements of the array.
         * @param count The count parameter is of type size_t and represents the number of times the
         * elements in the RepeatArray should be repeated.
         */
        Iterator(const RepeatArray<T>& array, size_t count) : _array(array), _count(count) {}

        /**
         * The function checks if the count of two iterators are not equal.
         *
         * @param other The "other" parameter is a reference to another Iterator object.
         *
         * @return a boolean value.
         */
        bool operator!=(const Iterator& other) const { return this->_count != other._count; }

        /**
         * The function returns the value of the array.
         *
         * @return The value of the `array` member variable is being returned.
         */
        T operator*() const { return this->_array._value; }

        /**
         * The above function overloads the pre-increment operator for an Iterator class,
         * incrementing the count variable and returning a reference to the updated object.
         *
         * @return The iterator object itself is being returned.
         */
        Iterator& operator++() {
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
     * The end() function returns an iterator pointing to the end of the container.
     *
     * @return The end iterator of the container.
     */
    Iterator end() const { return Iterator(*this, this->_size); }
};

/* The above code is implementing a template class called ShiftArray in C++. */
template <typename T> class ShiftArray {
  private:
    size_t _start = 0;
    std::vector<T> _lst;

  public:
    /**
     * The ShiftArray function takes a vector as input and initializes the start variable to 0 and
     * the lst variable to the input vector.
     *
     * @param lst The parameter "lst" is a vector of type T, which is the type of elements stored in
     * the vector.
     */
    explicit ShiftArray(std::vector<T> lst) : _lst(lst) {}

    void set_start(size_t start) { this->_start = start; }

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
     * The code defines a class Iterator that allows iteration over elements in a ShiftArray.
     *
     * @return The code is returning an iterator object.
     */
    class Iterator {
      private:
        const ShiftArray<T>& _array;
        size_t _count;

      public:
        /**
         * The Iterator constructor takes a ShiftArray object and a count as parameters.
         *
         * @param array The "array" parameter is of type "ShiftArray<T>", which is a template class
         * representing an array that can be shifted (rotated) to the left or right. It is passed by
         * reference to the constructor of the "Iterator" class.
         * @param count The count parameter is of type size_t and represents the number of elements
         * in the ShiftArray that the Iterator will iterate over.
         */
        Iterator(const ShiftArray<T>& array, size_t count) : _array(array), _count(count) {}

        /**
         * The function checks if the count of two iterators are not equal.
         *
         * @param other The "other" parameter is a reference to another Iterator object.
         *
         * @return a boolean value.
         */
        bool operator!=(const Iterator& other) const { return this->_count != other._count; }

        /**
         * The function returns the value at the current position in an array.
         *
         * @return The code is returning the element at the index `array.start + count` in the
         * `array.lst` array.
         */
        T operator*() const { return this->_array._lst[this->_array._start + this->_count]; }

        /**
         * The above function overloads the pre-increment operator for an Iterator class,
         * incrementing the count variable and returning a reference to the updated object.
         *
         * @return The iterator object itself is being returned.
         */
        Iterator& operator++() {
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
    Iterator end() const { return Iterator(*this, this->_lst.size() - this->_start); }

    /**
     * The function returns the size of a list, excluding a specified starting index.
     *
     * @return the size of the list (lst) minus the starting index (start).
     */
    size_t size() const { return this->_lst.size() - this->_start; }

    /**
     * The function returns an iterator object that allows iterating over the items in the current
     * object.
     *
     * @return An iterator object is being returned.
     */
    Iterator items() const { return Iterator(*this, 0); }
};

int main() {
    RepeatArray<int> arr(1, 10);
    std::cout << arr[4] << std::endl;
    for (int i : arr) {
        std::cout << i << std::endl;
    }

    ShiftArray<int> a({2, 3, 5, 7, 11});
    a.set_start(5);
    std::cout << a[6] << std::endl;
    for (auto it = a.items(); it != a.end(); ++it) {
        std::cout << *it << std::endl;
    }

    return 0;
}
