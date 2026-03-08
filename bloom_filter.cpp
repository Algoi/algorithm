/**
 * bloom_filter.cpp
 *
 * Bloom Filter implementation for approximate set membership queries.
 * Supports configurable expected item count and target false positive rate.
 *
 * Interface:
 *   BloomFilter filter(expected_items, false_positive_rate)
 *   filter.add(value)
 *   filter.contains(value)
 *   filter.clear()
 *
 * Notes:
 *   - False positives are possible, false negatives are not.
 *   - Uses double hashing to derive k hash functions from two 64-bit hashes.
 */

#include <cmath>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class BloomFilter {
public:
    BloomFilter(std::size_t expected_items, double false_positive_rate)
        : bit_count_(optimal_bit_count(expected_items, false_positive_rate)),
          hash_count_(optimal_hash_count(bit_count_, expected_items)),
          bits_((bit_count_ + 63) / 64, 0)
    {
        if (expected_items == 0) {
            throw std::invalid_argument("expected_items must be greater than 0");
        }
        if (false_positive_rate <= 0.0 || false_positive_rate >= 1.0) {
            throw std::invalid_argument("false_positive_rate must be in (0, 1)");
        }
    }

    template <typename T>
    void add(const T& value)
    {
        const auto [hash1, hash2] = make_hashes(value);
        for (std::size_t index = 0; index < hash_count_; ++index) {
            set_bit(nth_hash(hash1, hash2, index));
        }
    }

    template <typename T>
    bool contains(const T& value) const
    {
        const auto [hash1, hash2] = make_hashes(value);
        for (std::size_t index = 0; index < hash_count_; ++index) {
            if (!get_bit(nth_hash(hash1, hash2, index))) {
                return false;
            }
        }
        return true;
    }

    void clear()
    {
        std::fill(bits_.begin(), bits_.end(), 0);
    }

    std::size_t bit_count() const
    {
        return bit_count_;
    }

    std::size_t hash_count() const
    {
        return hash_count_;
    }

private:
    static std::uint64_t splitmix64(std::uint64_t value)
    {
        value += 0x9e3779b97f4a7c15ULL;
        value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
        value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
        return value ^ (value >> 31);
    }

    template <typename T>
    static std::pair<std::uint64_t, std::uint64_t> make_hashes(const T& value)
    {
        const std::uint64_t hash1 = static_cast<std::uint64_t>(std::hash<T>{}(value));
        std::uint64_t hash2 = splitmix64(hash1 ^ 0x27d4eb2f165667c5ULL);
        if (hash2 == 0) {
            hash2 = 0x9e3779b97f4a7c15ULL;
        }
        return {hash1, hash2};
    }

    static std::size_t optimal_bit_count(std::size_t expected_items, double false_positive_rate)
    {
        const double numerator = -static_cast<double>(expected_items) * std::log(false_positive_rate);
        const double denominator = std::log(2.0) * std::log(2.0);
        return static_cast<std::size_t>(std::ceil(numerator / denominator));
    }

    static std::size_t optimal_hash_count(std::size_t bit_count, std::size_t expected_items)
    {
        const double raw = (static_cast<double>(bit_count) / static_cast<double>(expected_items)) * std::log(2.0);
        return std::max<std::size_t>(1, static_cast<std::size_t>(std::round(raw)));
    }

    std::size_t nth_hash(std::uint64_t hash1, std::uint64_t hash2, std::size_t index) const
    {
        return static_cast<std::size_t>((hash1 + index * hash2) % bit_count_);
    }

    void set_bit(std::size_t index)
    {
        bits_[index / 64] |= (1ULL << (index % 64));
    }

    bool get_bit(std::size_t index) const
    {
        return (bits_[index / 64] & (1ULL << (index % 64))) != 0;
    }

    std::size_t bit_count_;
    std::size_t hash_count_;
    std::vector<std::uint64_t> bits_;
};

int main()
{
    BloomFilter filter(1000, 0.01);

    const std::vector<std::string> inserted = {
        "apple", "banana", "cherry", "grape", "orange"
    };

    for (const auto& item : inserted) {
        filter.add(item);
    }

    const std::vector<std::string> queries = {
        "apple", "banana", "watermelon", "peach", "grape"
    };

    std::cout << "Bloom Filter demo\n";
    std::cout << "bit count : " << filter.bit_count() << '\n';
    std::cout << "hash count: " << filter.hash_count() << "\n\n";

    for (const auto& item : queries) {
        std::cout << std::left << std::setw(12) << item
                  << (filter.contains(item) ? "possibly present" : "definitely absent")
                  << '\n';
    }

    return 0;
}
