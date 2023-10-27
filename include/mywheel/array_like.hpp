#pragma once

// #include <any>
#include <cassert>
#include <cstddef>
#include <utility>

class RepeatArray {
  private:
    int _value;
    size_t _size;

  public:
    RepeatArray(int value, size_t size) : _value{value}, _size{size} {}

    int operator[](int index) { return _value; }

    size_t size() { return _size; }

    class Iterator {
      private:
        int _value;
        size_t _count = 0;

      public:
        Iterator(int value, size_t count) : _value{value}, _count{count} {}

        bool operator!=(const Iterator &other) const { return _count != other._count; }

        int operator*() const { return _value; }

        Iterator &operator++() {
            _count++;
            return *this;
        }
    };

    Iterator begin() { return Iterator(_value, 0); }

    Iterator end() { return Iterator(_value, _size); }
};

/**
 * @brief
 *
 * @tparam Container
 */

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
 * @tparam Container
 */
template <typename Container> class ShiftArray : public Container {
    using value_type = typename Container::value_type;

  private:
    size_t _start{0U};

  public:
    /**
     * @brief Construct a new Shift Array object
     *
     */
    ShiftArray() : Container{} {}

    /**
     * @brief Construct a new Shift Array object
     *
     * @param base
     */
    explicit ShiftArray(Container &&base) : Container{std::forward<Container>(base)} {}

    /**
     * @brief Set the start object
     *
     * @param start
     */
    void set_start(const size_t &start) { this->_start = start; }

    /**
     * @brief
     *
     * @param index
     * @return const value_type&
     */
    auto operator[](const size_t &index) const -> const value_type & {
        assert(index >= this->_start);
        return Container::operator[](index - this->_start);
    }

    /**
     * @brief
     *
     * @param index
     * @return value_type&
     */
    auto operator[](const size_t &index) -> value_type & {
        return Container::operator[](index - this->_start);
    }
};
