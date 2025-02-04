/**
 * @file RichText.h
 * @author Grant
 *
 */

#pragma once

#include <optional>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include "index_set.hpp"

/* PLACEHOLDERS
 * These classes don't exist yet, but are used as placeholders for this prototype.
 * DON'T PUSH THIS TO MAIN!!!
 * TODO: Replace placeholders
 * */
namespace cse {
    struct Datum{ // From group 5
        std::string data;
    };
}

namespace cse {
    class RichText {
    public:
        struct Format{
            std::string name;
            cse::Datum metadata; // From group 5
        };

    private:
        std::string text;
        std::map<Format, cse::IndexSet> formatting;

    public:

        size_t size() const noexcept;
        const char& char_at (size_t pos) const;
        std::unique_ptr<std::vector<const Format*>> format_at (size_t pos) const;
        RichText& append (const RichText& str);
        RichText& operator+= (const RichText& str);

        std::optional<const cse::IndexSet* const> getFormatRange (const Format&) const;
    };

    bool operator<(const RichText::Format& l, const RichText::Format& r);
}
