/**
 * @file RichText.cpp
 * @author Grant
 */


#include "../include/cse/RichText.hpp"
#include "index_set.cpp"

namespace cse {

    bool operator<(const RichText::Format& l, const RichText::Format& r){
        if (l.name == r.name) return l.metadata.data < r.metadata.data;
        return l.name < r.name;
    }

    size_t RichText::size() const noexcept
    {
        return text.size();
    }

    const char& RichText::char_at(size_t pos) const
    {
        return text.at(pos);
    }

    RichText& RichText::append(const RichText& str)
    {
        text += str.text;
        //TODO:
        //For every Format in formatting
        //Match it with a format in str
        //Offset the IndexSet in str
        //Add the offset IndexSet to the existing IndexSet
        return *this;
    }

    RichText& RichText::operator+=(const RichText& str)
    {
        return append(str);
    }

    std::optional<const IndexSet* const> RichText::getFormatRange(const RichText::Format& format) const
    {
        auto iter = formatting.find(format);
        if (iter == formatting.end()) return {};
        return {&iter->second};
    }

    std::unique_ptr<std::vector<const RichText::Format*>> RichText::format_at(size_t pos) const
    {
        auto result = std::make_unique<std::vector<const RichText::Format*>>();
            for( auto& pair : formatting){
                if(pair.second.contains(pos)) result->push_back(&pair.first);
            }
        return result;
    }


}

// Quick tests
int main(){

}