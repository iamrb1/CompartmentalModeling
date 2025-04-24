#include "../../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../Project Source/Classes/Backend Main/Card.hpp"
#include "../Project Source/Classes/Backend Main/CardHash.hpp"
#include "../Project Source/Classes/Backend Main/Board.hpp"

// This file contains tests for the Board and Card classes. All other classes should have significant enough tests for the final project.
TEST_CASE("Final Project Tests", "[BoardCardCardhash]") {
    SECTION("Card Basic Functionality") {
        cse::Card card1(1, "Initial content");
        CHECK(card1.getId() == 1);
        CHECK(card1.getContent() == "Initial content");
        
        card1.setContent("New content");
        CHECK(card1.getContent() == "New content");
        
        card1.clearContent();
        CHECK(card1.getContent().empty());
    }
    
    SECTION("Card Tag Management") {
        cse::Card card1(1, "Content");
        CHECK_FALSE(card1.hasTag("important"));
        
        card1.addTag("important");
        CHECK(card1.hasTag("important"));
        CHECK(card1.getTags().size() == 1);
        CHECK(card1.getTags()[0] == "important");
        
        card1.addTag("urgent");
        CHECK(card1.hasTag("urgent"));
        CHECK(card1.getTags().size() == 2);
        
        card1.removeTag("important");
        CHECK_FALSE(card1.hasTag("important"));
        CHECK(card1.getTags().size() == 1);
        
        card1.removeTag("nonexistent");
        CHECK(card1.getTags().size() == 1);
    }
    
    SECTION("Card Equality Comparison") {
        cse::Card card1(1, "Content");
        cse::Card card2(1, "Different content");
        cse::Card card3(2, "Content");
        
        CHECK(card1 == card2);
        CHECK_FALSE(card1 == card3);
    }
    
    SECTION("Board Basic Operations") {
        cse::Board board;
        CHECK(board.size() == 0);
        
        cse::Card card1(1, "First card");
        CHECK(board.addCard(card1));
        CHECK(board.size() == 1);
        
        cse::Card card2(2, "Second card");
        CHECK(board.addCard(card2));
        CHECK(board.size() == 2);
        
        CHECK_FALSE(board.addCard(card1));
        CHECK(board.size() == 2);
        
        CHECK(board.getCard(0).getId() == 1);
        CHECK(board.getCard(1).getId() == 2);
        
        CHECK(board.removeCard(card1));
        CHECK(board.size() == 1);
        CHECK(board.getCard(0).getId() == 2);
        
        CHECK_FALSE(board.removeCard(card1));
        CHECK(board.size() == 1);
    }
    
    SECTION("Card Hash Functionality") {
        cse::Card card1(1, "Content");
        cse::Card card2(1, "Different content");
        cse::Card card3(2, "Content");
        
        std::hash<cse::Card> hasher;
        CHECK(hasher(card1) == hasher(card2));
        CHECK_FALSE(hasher(card1) == hasher(card3));
        
        CHECK(hasher(card1) == std::hash<int>()(1));
        CHECK(hasher(card3) == std::hash<int>()(2));
    }
    
    SECTION("Board Edge Cases") {
        cse::Board board;
        cse::Card defaultCard;
        
        CHECK_FALSE(board.removeCard(defaultCard));
        CHECK_THROWS_AS(board.getCard(0), std::out_of_range);
        
        cse::Card card1(1, "Card");
        CHECK(board.addCard(card1));
        CHECK_THROWS_AS(board.getCard(1), std::out_of_range);
    }
    
    SECTION("Card Content Modification") {
        cse::Card card(1, "Initial");
        card.setContent("Modified");
        CHECK(card.getContent() == "Modified");
        
        card.clearContent();
        CHECK(card.getContent().empty());
        
        card.setContent("New");
        CHECK(card.getContent() == "New");
    }
    
    SECTION("Multiple Tags Management") {
        cse::Card card(1, "Content");
        card.addTag("tag1");
        card.addTag("tag2");
        card.addTag("tag3");
        
        auto tags = card.getTags();
        CHECK(tags.size() == 3);
        CHECK(std::find(tags.begin(), tags.end(), "tag1") != tags.end());
        CHECK(std::find(tags.begin(), tags.end(), "tag2") != tags.end());
        CHECK(std::find(tags.begin(), tags.end(), "tag3") != tags.end());
        
        card.removeTag("tag2");
        tags = card.getTags();
        CHECK(tags.size() == 2);
        CHECK(std::find(tags.begin(), tags.end(), "tag2") == tags.end());
    }
    
    SECTION("Board Stress Test") {
        cse::Board board;
        const int count = 100;
        
        for (int i = 0; i < count; ++i) {
            cse::Card card(i, "Card " + std::to_string(i));
            CHECK(board.addCard(card));
        }
        
        CHECK(board.size() == count);
        
        for (int i = 0; i < count; ++i) {
            CHECK(board.getCard(i).getId() == i);
        }
        
        for (int i = count - 1; i >= 0; --i) {
            cse::Card card(i, "");
            CHECK(board.removeCard(card));
            CHECK(board.size() == i);
        }
    }
}