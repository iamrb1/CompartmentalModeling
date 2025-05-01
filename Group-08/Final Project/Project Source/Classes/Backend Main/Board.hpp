#ifndef CSE_BOARD_H
#define CSE_BOARD_H

// #include "DynamicString.hpp"
// #include "TaskColumn.hpp"
#include "../Backend Support/RandomAccessSet/RandomAccessSet.hpp"
#include "Card.hpp"
#include "CardHash.hpp"

namespace cse {

    /**
     * @class   Board
     * @file    Board.hpp
     * @author  Joanna Rodriguez Zamarron
     * @brief   Represents a board that manages a set of unique Card objects with random access.
     * 
     * This class acts as the container for all cards on the task board. It allows
     * adding, removing, and accessing cards using index-based retrieval while ensuring
     * that each card is unique. Internally, it leverages RandomAccessSet to manage
     * both uniqueness and fast access.
     */
class Board {
    private:
        /**
         * @brief A random-access set of unique cards stored in the board.
         */
        cse::RandomAccessSet<Card> cards;
    
    public:
        /**
         * @brief Default constructor.
         */
        Board() = default;
    
        /**
         * @brief Adds a new card to the board.
         * 
         * @param card The Card to be added.
         * @return True if the card was successfully added; false if it already exists.
         */
        bool addCard(const Card& card) {
            return cards.add(card);
        }
    
        /**
         * @brief Removes an existing card from the board.
         * 
         * @param card The Card to be removed.
         * @return True if the card was successfully removed; false if it was not found.
         */
        bool removeCard(const Card& card) {
            return cards.remove(card);
        }
    
        /**
         * @brief Retrieves a card by its index in the board.
         * 
         * @param index The index of the card to retrieve.
         * @return The Card at the specified index.
         * @throws std::out_of_range if index is invalid.
         */
        Card getCard(size_t index) const {
            return cards.get(index);
        }
        
        /**
         * @brief Returns the total number of cards on the board.
         * 
         * @return The size of the board.
         */
        size_t size() const {
            return cards.size();
        }
};
}
    
#endif
