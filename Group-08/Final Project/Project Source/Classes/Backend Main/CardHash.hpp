#pragma once
#include "Card.hpp"
#include <functional>

namespace std {
    template <>
    struct hash<cse::Card> {
        std::size_t operator()(const cse::Card& card) const noexcept {
            return std::hash<int>()(card.getId());
        }
    };
}
