#include "TokenManager.hpp"

namespace cse {

void TokenManager::Clean() {
  if(mToken_id) {
    mTokens.clear();
    //mTokens.erase(mTokens.begin(), mTokens.end());
    mToken_id = 0;
  }
}

TokenManager::TokenManager() : mToken_id(0) {}

TokenManager::~TokenManager() {
  Clean();
}

void TokenManager::Load(const std::string& input) {
  Clean();                 
  mTokens = mLexer.Tokenize(input);
}

const emplex::Token & TokenManager::Peek() const {
    if ( mToken_id < mTokens.size()) {
        return mTokens[mToken_id];
    }
    return eof_token;
}

const emplex::Token & TokenManager::Use() {
    if (mToken_id < mTokens.size()) {
        return mTokens[mToken_id++];
    }
    return eof_token;
}

const emplex::Token & TokenManager::Use_if(int expectedId) {
    if (mToken_id < mTokens.size() && mTokens[mToken_id].id == expectedId) {
        return mTokens[mToken_id++];
    }
    return eof_token;
}

} // namespace cse
