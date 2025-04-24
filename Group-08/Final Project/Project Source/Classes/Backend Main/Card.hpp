#ifndef CSE_CARD_H
#define CSE_CARD_H

#include "../Backend Support/RandomAccessSet/RandomAccessSet.hpp"
#include "../Backend Support/AnnotatedWrapper/AnnotatedWrapper.hpp"
#include "../Backend Support/AuditedPointer/AuditedPointer.hpp"

#include "../Backend Support/DynamicString/DynamicString.hpp"
#include "../Backend Support/TagManager/TagManager.hpp"

// Test class currently without DynamicString or TagManager
namespace cse {

class Card : public AnnotatedWrapper<std::string> {
    private:
    int mId;
    // Dynamic string holds the card's content
    cse::Aptr<cse::DynamicString> mContent;
     // Tag manager to manage card tags
    TagManager mTags;
    
    public:
    Card() = default;

    ~Card() {
        mContent.Delete();
    }
    
    Card(int id, const std::string& content)
        : mId(id), mContent(new DynamicString()) {
        mContent->Append(content); // Initial content added to dynamic string
    }

    int getId() const { return mId; }

    std::string getContent() const {
        return mContent->ToString();
    }

    void setContent(const std::string& newContent) {
        mContent->Clear();
        mContent->Append(newContent);
    }

    void addTag(const std::string& tag) {
        mTags.addTag(std::to_string(mId), tag);
    }

    void removeTag(const std::string& tag) {
        mTags.removeTag(std::to_string(mId), tag);
    }

    bool hasTag(const std::string& tag) const {
        return mTags.hasTag(std::to_string(mId), tag);
    }

    std::vector<std::string> getTags() const {
        return mTags.getTags(std::to_string(mId));
    }

    void clearContent() {
        mContent->Clear();
    }

    bool operator==(const Card& other) const {
        return mId == other.mId;
    }
};
}

#endif