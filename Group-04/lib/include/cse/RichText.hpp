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
#include <variant>
#include <vector>
#include "index_set.hpp"

namespace cse {
    class RichText {
    public:
        struct Format{
            std::string name;
            std::variant<std::string, int32_t> metadata;
        };

    private:
        std::string text;
        std::map<Format, cse::IndexSet> formatting;

    public:

        size_t size() const noexcept
        {
            return text.size();
        }

        const char& char_at(size_t pos) const
        {
            return text.at(pos);
        }

        std::unique_ptr<std::vector<const Format*>> format_at(size_t pos) const
        {
            auto result = std::make_unique<std::vector<const Format*>>();
            for( auto& pair : formatting){
                if(pair.second.contains(pos)) result->push_back(&pair.first);
            }
            return result;
        }

        RichText& append(const RichText& str)
        {
            text += str.text;
            //TODO:
            //For every Format in formatting
            //Match it with a format in str
            //Offset the IndexSet in str
            //Add the offset IndexSet to the existing IndexSet
            return *this;
        }

        RichText& operator+=(const RichText& str)
        {
            return append(str);
        }

        std::optional<const IndexSet* const> getFormatRange(const Format& format) const
        {
            auto iter = formatting.find(format);
            if (iter == formatting.end()) return {};
            return {&iter->second};
        }
    };

    inline bool operator<(const RichText::Format& l, const RichText::Format& r){
        if (l.name == r.name) return l.metadata < r.metadata;
        return l.name < r.name;
    }
}