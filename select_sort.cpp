/**
 * select_sort.cpp
 *
 * Generic Selection Sort implementation with comparator support.
 * Supports std::vector and raw arrays / pointers.
 *
 * Interface:
 *   select_sort(arr, size, cmp)
 *     - arr : raw array / pointer
 *     - size: number of elements
 *     - cmp : binary comparator — cmp(a, b) returns true when a should
 *             come before b
 *
 *   select_sort(vec, cmp)   -- std::vector overload with comparator
 *   select_sort(vec)        -- std::vector overload, ascending by default
 *
 * Notes:
 *   - In-place sorting algorithm.
 *   - Performs O(n^2) comparisons regardless of input order.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename T, typename Compare>
void select_sort(T* arr, std::size_t size, Compare cmp)
{
    if (size < 2) {
        return;
    }

    for (std::size_t left = 0; left + 1 < size; ++left) {
        std::size_t selected = left;
        for (std::size_t index = left + 1; index < size; ++index) {
            if (cmp(arr[index], arr[selected])) {
                selected = index;
            }
        }

        if (selected != left) {
            std::swap(arr[left], arr[selected]);
        }
    }
}

template <typename T, typename Compare>
void select_sort(std::vector<T>& vec, Compare cmp)
{
    select_sort(vec.data(), vec.size(), cmp);
}

template <typename T>
void select_sort(std::vector<T>& vec)
{
    select_sort(vec, [](const T& a, const T& b) { return a < b; });
}

struct Student {
    std::string name;
    int score;

    friend std::ostream& operator<<(std::ostream& os, const Student& student)
    {
        return os << "{" << student.name << ", " << student.score << "}";
    }
};

template <typename T>
void print(const std::string& label, const std::vector<T>& values)
{
    std::cout << label << ": [ ";
    for (const auto& value : values) {
        std::cout << value << " ";
    }
    std::cout << "]\n";
}

int main()
{
    std::vector<int> numbers = {9, 4, 7, 1, 3, 8, 2, 6, 5, 0};
    select_sort(numbers);
    print("int asc", numbers);

    std::vector<std::string> words = {"pear", "apple", "orange", "banana"};
    select_sort(words);
    print("str asc", words);

    std::vector<Student> students = {
        {"Alice", 88}, {"Bob", 75}, {"Carol", 93}, {"Dave", 82}
    };
    select_sort(students, [](const Student& left, const Student& right) {
        return left.score > right.score;
    });
    print("score desc", students);

    int raw[] = {5, 1, 4, 2, 8};
    select_sort(raw, sizeof(raw) / sizeof(raw[0]), [](int left, int right) {
        return left < right;
    });

    std::cout << "raw asc: [ ";
    for (int value : raw) {
        std::cout << value << " ";
    }
    std::cout << "]\n";

    return 0;
}
