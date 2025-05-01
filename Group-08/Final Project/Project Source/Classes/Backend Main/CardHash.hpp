#pragma once
#include "Card.hpp"
#include <functional>

/**
 * @file    CardHash.hpp
 * @brief   Hash function specialization for Card to allow use in unordered containers.
 * 
 * This file provides a specialization of std::hash for the cse::Card class,
 * enabling Card objects to be used as keys in hash-based containers like
 * std::unordered_map and std::unordered_set. The hash is based solely on the
 * card's unique ID, making it consistent with the Card's equality operator.
 */

namespace std {
    /**
     * @struct hash
     * @brief Specialization of std::hash for cse::Card.
     *
     * Generates a hash value based on the Card’s ID. This enables
     * usage of Card objects in standard hash containers.
     */
    template <>
    struct hash<cse::Card> {
        /**
         * @brief Computes a hash value for a given Card.
         * 
         * @param card The Card object to hash.
         * @return std::size_t The hash value based on the card’s ID.
         */
        std::size_t operator()(const cse::Card& card) const noexcept {
            return std::hash<int>()(card.getId());
        }
    };
}
