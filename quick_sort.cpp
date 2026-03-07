/**
 * quick_sort.cpp
 *
 * Generic Quick Sort implementation using a comparator interface.
 * Supports any type (built-in or custom) via template parameters.
 *
 * Interface:
 *   quick_sort(arr, left, right, cmp)
 *     - arr  : raw array / pointer
 *     - left : inclusive left index
 *     - right: inclusive right index
 *     - cmp  : binary comparator — cmp(a, b) returns true when a should
 *              come before b (strict weak ordering, same as std::sort)
 *
 *   quick_sort(vec, cmp)   -- std::vector overload with comparator
 *   quick_sort(vec)        -- std::vector overload, ascending by default
 *
 * Optimisations:
 *   - Median-of-three pivot selection  (avoids worst-case on sorted input)
 *   - Insertion sort for sub-arrays smaller than INSERTION_THRESHOLD (16)
 *   - Tail-call optimisation: always recurse on the smaller partition
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>   // std::swap

// ─────────────────────────────────────────────────────────────
//  Insertion sort  (used for small sub-arrays, cache-friendly)
// ─────────────────────────────────────────────────────────────
template <typename T, typename Compare>
void insertion_sort(T* arr, int left, int right, Compare cmp)
{
    for (int i = left + 1; i <= right; ++i) {
        T key = arr[i];
        int j = i - 1;
        while (j >= left && cmp(key, arr[j])) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// ─────────────────────────────────────────────────────────────
//  Median-of-three pivot selection
//  Rearranges arr[left], arr[mid], arr[right] so that arr[right]
//  holds the median value (used as pivot), then returns right.
// ─────────────────────────────────────────────────────────────
template <typename T, typename Compare>
int median_of_three(T* arr, int left, int right, Compare cmp)
{
    int mid = left + (right - left) / 2;
    if (cmp(arr[mid],  arr[left]))  std::swap(arr[mid],  arr[left]);
    if (cmp(arr[right], arr[left])) std::swap(arr[right], arr[left]);
    if (cmp(arr[right], arr[mid]))  std::swap(arr[right], arr[mid]);
    // arr[left] <= arr[mid] <= arr[right] in cmp ordering
    // Place median pivot just before arr[right] to keep it out of partitioning
    std::swap(arr[mid], arr[right - 1]);
    return right - 1;
}

// ─────────────────────────────────────────────────────────────
//  quick_sort — raw array / pointer overload
//
//  Template parameters:
//    T       : element type (any copyable / movable type)
//    Compare : callable with signature bool(const T&, const T&)
//              Returns true if first argument should come before second.
//              Examples:
//                [](int a, int b){ return a < b; }    // ascending
//                std::greater<int>{}                  // descending
//                [](const Foo& a, const Foo& b){ return a.key < b.key; }
// ─────────────────────────────────────────────────────────────
template <typename T, typename Compare>
void quick_sort(T* arr, int left, int right, Compare cmp)
{
    constexpr int INSERTION_THRESHOLD = 16;

    while (left < right) {
        if (right - left < INSERTION_THRESHOLD) {
            insertion_sort(arr, left, right, cmp);
            return;
        }

        int pivot_pos = median_of_three(arr, left, right, cmp);
        T   pivot     = arr[pivot_pos];

        // Two-pointer partition (pivot is parked at right-1)
        int i = left;          // will scan rightward
        int j = right - 1;    // will scan leftward

        while (true) {
            while (cmp(arr[++i], pivot)) {}   // advance while arr[i] < pivot
            while (cmp(pivot, arr[--j])) {}   // advance while pivot < arr[j]
            if (i >= j) break;
            std::swap(arr[i], arr[j]);
        }
        std::swap(arr[i], arr[right - 1]);    // restore pivot to its final place

        // Recurse on the smaller half; loop on the larger half (limits stack to O(log n))
        if (i - left < right - i) {
            quick_sort(arr, left,  i - 1, cmp);
            left  = i + 1;
        } else {
            quick_sort(arr, i + 1, right, cmp);
            right = i - 1;
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  Convenience overloads for std::vector
// ─────────────────────────────────────────────────────────────
template <typename T, typename Compare>
void quick_sort(std::vector<T>& vec, Compare cmp)
{
    if (vec.size() > 1)
        quick_sort(vec.data(), 0, static_cast<int>(vec.size()) - 1, cmp);
}

template <typename T>
void quick_sort(std::vector<T>& vec)
{
    quick_sort(vec, [](const T& a, const T& b) { return a < b; });
}

// =============================================================
//  Demo
// =============================================================

struct Student {
    std::string name;
    double      gpa;
    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        return os << "{" << s.name << ", " << s.gpa << "}";
    }
};

template <typename T>
void print(const std::string& label, const std::vector<T>& v)
{
    std::cout << label << ": [ ";
    for (const auto& e : v) std::cout << e << " ";
    std::cout << "]\n";
}

int main()
{
    // 1. int — ascending (default comparator)
    std::vector<int> a = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    quick_sort(a);
    print("int  asc ", a);

    // 2. int — descending (custom comparator lambda)
    std::vector<int> b = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    quick_sort(b, [](int x, int y) { return x > y; });
    print("int  desc", b);

    // 3. double
    std::vector<double> c = {3.14, 2.71, 1.41, 1.73, 0.57};
    quick_sort(c);
    print("dbl  asc ", c);

    // 4. std::string — lexicographic
    std::vector<std::string> d = {"banana", "apple", "cherry", "date"};
    quick_sort(d);
    print("str  asc ", d);

    // 5. Custom struct — sort by GPA (descending)
    std::vector<Student> students = {
        {"Alice", 3.8}, {"Bob", 3.5}, {"Carol", 3.9}, {"Dave", 3.2}
    };
    quick_sort(students, [](const Student& x, const Student& y) {
        return x.gpa > y.gpa;
    });
    print("Student (gpa  desc)", students);

    // 6. Custom struct — sort by name (ascending)
    quick_sort(students, [](const Student& x, const Student& y) {
        return x.name < y.name;
    });
    print("Student (name asc )", students);

    // 7. Raw C array — with explicit comparator
    int raw[] = {9, 1, 5, 3, 7, 2, 8, 4, 6, 0};
    int n     = static_cast<int>(sizeof(raw) / sizeof(raw[0]));
    quick_sort(raw, 0, n - 1, [](int x, int y) { return x < y; });
    std::cout << "raw  asc : [ ";
    for (int x : raw) std::cout << x << " ";
    std::cout << "]\n";

    return 0;
}
