#ifndef CSE_CARD_H
#define CSE_CARD_H

#include "../Backend Support/RandomAccessSet/RandomAccessSet.hpp"
#include "../Backend Support/AnnotatedWrapper/AnnotatedWrapper.hpp"
#include "../Backend Support/AuditedPointer/AuditedPointer.hpp"

#include "../Backend Support/DynamicString/DynamicString.hpp"
#include "../Backend Support/TagManager/TagManager.hpp"

/**
 * @class   Card
 * @file    Card.hpp
 * @author  Joanna Rodriguez
 * @brief   Represents a task or note on the board, with unique ID, editable content,
 *          and optional tags. Inherits annotation support from AnnotatedWrapper.
 *
 * Each Card manages:
 * - A unique ID
 * - Editable content using DynamicString
 * - Tags for organization and search via TagManager
 * - Metadata annotations from the AnnotatedWrapper base class
 */
namespace cse {

class Card : public AnnotatedWrapper<std::string> {
    private:
    /// Unique identifier for the card
    int mId;
    // Smart pointer managing the card's editable text
    cse::Aptr<cse::DynamicString> mContent;
    /// Manages the tags associated with this card
    TagManager mTags;
    
    public:
    /**
     * @brief Default constructor.
     *        Creates an empty card with no ID or content.
     */
    Card() = default;

    
    /**
     * @brief Destructor.
     *        Ensures dynamic content is properly deleted and tracked.
     */
    ~Card() {
        mContent.Delete();
    }
    
    /**
     * @brief Constructs a card with a given ID and initial content.
     * @param id Unique ID for this card
     * @param content Initial string to populate the card's content
     */
    Card(int id, const std::string& content)
        : mId(id), mContent(new DynamicString()) {
        mContent->Append(content); // Initial content added to dynamic string
    }

    /**
     * @brief Returns the card's unique ID.
     * @return Integer representing the card's ID
     */
    int getId() const { return mId; }

    /**
     * @brief Retrieves the current content of the card.
     * @return A string representing the card's content
     */
    std::string getContent() const {
        return mContent->ToString();
    }

    /**
     * @brief Replaces the current content with a new string.
     * @param newContent New content to assign to the card
     */
    void setContent(const std::string& newContent) {
        mContent->Clear();
        mContent->Append(newContent);
    }

    /**
     * @brief Appends a tag to this card.
     * @param tag A string label to associate with the card
     */
    void addTag(const std::string& tag) {
        mTags.addTag(std::to_string(mId), tag);
    }

    /**
     * @brief Removes a tag from this card.
     * @param tag The tag to remove
     */
    void removeTag(const std::string& tag) {
        mTags.removeTag(std::to_string(mId), tag);
    }

    /**
     * @brief Checks if a tag is associated with the card.
     * @param tag The tag to check
     * @return True if the tag exists, false otherwise
     */
    bool hasTag(const std::string& tag) const {
        return mTags.hasTag(std::to_string(mId), tag);
    }

    /**
     * @brief Returns all tags currently associated with the card.
     * @return A vector of tag strings
     */
    std::vector<std::string> getTags() const {
        return mTags.getTags(std::to_string(mId));
    }
    
    /**
     * @brief Clears all content from the card.
     */
    void clearContent() {
        mContent->Clear();
    }

    /**
     * @brief Equality operator to compare two cards by their ID.
     * @param other The card to compare with
     * @return True if both cards have the same ID
     */
    bool operator==(const Card& other) const {
        return mId == other.mId;
    }
};
}

#endif