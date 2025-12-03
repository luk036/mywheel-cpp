#pragma once

#include <py2cpp/enumerate.hpp>
#include <py2cpp/range.hpp>
#include <type_traits>
#include <utility>
// #include <vector>

/**
 * @brief Dict-like data structure by std::vector and Range
 *
 * ```cpp
 * MapAdapter Structure:
 *
 * Original Container: [A] [B] [C] [D] [E]
 *                    [0] [1] [2] [3] [4]  <-- Indices become keys
 *
 * MapAdapter Interface:
 *
 * operator[](0) → A
 * operator[](1) → B
 * operator[](2) → C
 * operator[](3) → D
 * operator[](4) → E
 *
 * begin() → iterator to (0, A)
 * end()   → iterator past (4, E)
 *
 * enumerate() → [(0, A), (1, B), (2, C), (3, D), (4, E)]
 * ```cpp
 *
 * @tparam Container
 */
template <typename Container> class MapAdapter {
    using is_const_container = std::is_const<Container>;

  public:
    using key_type = size_t;
    using mapped_type = typename Container::value_type;
    using value_type = std::pair<key_type, mapped_type>;
    using E = decltype(py::enumerate(std::declval<Container &>()));

  private:
    py::Range<key_type> _rng;
    Container &_lst;
    E mapview;

  public:
    explicit MapAdapter(Container &lst)
        : _rng{py::range(lst.size())}, _lst(lst), mapview(py::enumerate(this->_lst)) {}

    const mapped_type &operator[](const key_type &key) const { return _lst.at(key); }

    template <bool dependent = true,
              typename = std::enable_if_t<dependent && !is_const_container::value>>
    mapped_type &operator[](const key_type &key) {
        return _lst[key];
    }

    /**
     * The function at() returns a constant reference to the value associated with a given key in a
     * map.
     *
     * @return a constant reference to the value associated with the given key.
     */
    const mapped_type &at(const key_type &key) const { return _lst.at(key); }

    /**
     * The function checks if a given key is present in a data structure.
     *
     * @param[in] key The parameter "key" is of type "key_type", which is a data type that
     * represents the key used to search for an element in the container.
     *
     * @return a boolean value. It will return true if the key is contained in the data structure,
     * and false otherwise.
     */
    bool contains(const key_type &key) const { return _rng.contains(key); }

    /**
     * The size() function returns the size of the _rng container.
     *
     * @return The size of the `_rng` object is being returned.
     */
    size_t size() const { return _rng.size(); }

    /**
     * The function returns an iterator pointing to the beginning of the mapview.
     *
     * @return The `begin()` function is returning an iterator pointing to the beginning of the
     * `mapview` container.
     */
    auto begin() const { return mapview.begin(); }

    /**
     * The function returns an iterator pointing to the end of the mapview.
     *
     * @return The end iterator of the mapview.
     */
    auto end() const { return mapview.end(); }
};

/**
 * @brief Dict-like data structure by std::vector and Range
 *
 * @tparam Container
 */
template <class T> using MapConstAdapter = MapAdapter<const T>;
