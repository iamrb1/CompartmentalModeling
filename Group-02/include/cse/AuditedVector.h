/**
 * @file AuditedVector.h
 * @author Matthew Hawkins
 */

#ifndef AUDITEDVECTOR_H
#define AUDITEDVECTOR_H

#include <vector>
#include <stdexcept>
#include <cstddef>

namespace cse {

#ifdef NDEBUG
    // In release mode, just use std::vector.
    template <typename T, typename Allocator = std::allocator<T>>
    using vector = std::vector<T, Allocator>;

#else
    template <typename T, typename Allocator = std::allocator<T>>
    class AuditedVector : public std::vector<T, Allocator> {
    public:
        using base_t = std::vector<T, Allocator>;
        using base_t::base_t;

        T& operator[](size_t index) {
            if (index >= this->size()) {
                throw std::out_of_range("cse::vector: index out of range");
            }
            return base_t::operator[](index);
        }

        const T& operator[](size_t index) const {
            if (index >= this->size()) {
                throw std::out_of_range("cse::vector: index out of range");
            }
            return base_t::operator[](index);
        }

        T& front() {
            if (this->empty()) {
                throw std::out_of_range("cse::vector: front() called on empty vector");
            }
            return base_t::front();
        }

        const T& front() const {
            if (this->empty()) {
                throw std::out_of_range("cse::vector: front() called on empty vector");
            }
            return base_t::front();
        }

        T& back() {
            if (this->empty()) {
                throw std::out_of_range("cse::vector: back() called on empty vector");
            }
            return base_t::back();
        }

        const T& back() const {
            if (this->empty()) {
                throw std::out_of_range("cse::vector: back() called on empty vector");
            }
            return base_t::back();
        }

    };

#endif

}  // End cse namespace

#endif
