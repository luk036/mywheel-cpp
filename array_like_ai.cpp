#include <iostream>
#include <vector>

template <typename T>
class RepeatArray {
private:
    T value;
    size_t size;

public:
    RepeatArray(T value, size_t size) : value(value), size(size) {}

    T operator[](size_t index) const {
        return value;
    }

    size_t len() const {
        return size;
    }

    class Iterator {
    private:
        const RepeatArray<T>& array;
        size_t count;

    public:
        Iterator(const RepeatArray<T>& array, size_t count) : array(array), count(count) {}

        bool operator!=(const Iterator& other) const {
            return count != other.count;
        }

        T operator*() const {
            return array.value;
        }

        Iterator& operator++() {
            count++;
            return *this;
        }
    };

    Iterator begin() const {
        return Iterator(*this, 0);
    }

    Iterator end() const {
        return Iterator(*this, size);
    }
};

template <typename T>
class ShiftArray {
private:
    size_t start;
    std::vector<T> lst;

public:
    ShiftArray(std::vector<T> lst) : start(0), lst(lst) {}

    void set_start(size_t start) {
        this->start = start;
    }

    const T& operator[](size_t key) const {
        return lst[key - start];
    }

    T& operator[](size_t key) {
        return lst[key - start];
    }

    class Iterator {
    private:
        const ShiftArray<T>& array;
        size_t count;

    public:
        Iterator(const ShiftArray<T>& array, size_t count) : array(array), count(count) {}

        bool operator!=(const Iterator& other) const {
            return count != other.count;
        }

        T operator*() const {
            return array.lst[array.start + count];
        }

        Iterator& operator++() {
            count++;
            return *this;
        }
    };

    Iterator begin() const {
        return Iterator(*this, 0);
    }

    Iterator end() const {
        return Iterator(*this, lst.size() - start);
    }

    size_t len() const {
        return lst.size() - start;
    }

    Iterator items() const {
        return Iterator(*this, 0);
    }
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
