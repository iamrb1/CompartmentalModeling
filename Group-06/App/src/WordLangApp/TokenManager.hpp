#ifndef CSE_TOKENMANAGER_HPP_
#define CSE_TOKENMANAGER_HPP_

#include <vector>

#include "lexer.hpp"

namespace cse {
  class TokenManager {
  public:
    const emplex::Token eof_token{0, "_EOF_", 0, 0};
    
    TokenManager();
    ~TokenManager();

    void Load(const std::string& input);

    const emplex::Token & Peek() const;

    const emplex::Token & Use();

    const emplex::Token & Use_if(int expectedId);

  private:
    emplex::Lexer mLexer;

    std::vector<emplex::Token> mTokens{};

    std::size_t mToken_id = 0;

    void Clean();
  };

}

#endif  // CSE_TOKENMANAGER_HPP_