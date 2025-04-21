/**
 * @file TokenManager.hpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Orhan Aydin
 * @author Emil Rahn Siegel
 * @brief TokenManager header file.
 * @version 0.1
 * @date 2025-04-19
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef CSE_TOKENMANAGER_HPP_
#define CSE_TOKENMANAGER_HPP_

#include <vector>

#include "lexer.hpp"

// CITE: This class is derived from CSE450 TokenQueue class.
// https://github.com/MSU-CSE450/Project3-Instructor/blob/main/TokenQueue.hpp

namespace cse {
/**
 * @brief Utility class that handles tokenization and managements of tokens
 *
 */
class TokenManager {
 public:
  /// @brief Static end of file token
  const emplex::Token eof_token{0, "_EOF_", 0, 0};

  /**
   * @brief Construct a new Token Manager object
   *
   */
  TokenManager();

  /**
   * @brief Destroy the Token Manager object
   *
   */
  ~TokenManager();

  /**
   * @brief Load user input from terminal and tokenize it
   *
   * @param input String users terminal input
   */
  void Load(const std::string& input);

  /**
   * @brief Get the next token but do not use it
   *
   * @return const emplex::Token& Next token in the queue
   */
  const emplex::Token& Peek() const;

  /**
   * @brief Use the current token and skip to the next
   *
   * @return const emplex::Token& Current token that used
   */
  const emplex::Token& Use();

  /**
   * @brief Use token if id matched.
   *
   * @param expectedId Int id of token
   * @return const emplex::Token& used token.
   */
  const emplex::Token& Use_if(int expectedId);

 private:
  /// @brief Lexer which handles tokenization
  emplex::Lexer mLexer;

  /// @brief Collection of tokens
  std::vector<emplex::Token> mTokens{};

  /// @brief Index of current token
  std::size_t mToken_id = 0;

  /**
   * @brief Clear the token container
   *
   * Clears the token queue, helps us to use this class over and over instead
   * of new object creation for each line.
   */
  void Clean();
};

}  // namespace cse

#endif  // CSE_TOKENMANAGER_HPP_