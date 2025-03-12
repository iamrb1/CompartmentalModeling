/**
 * @file StringSet.cpp
 * @author Orhan Aydin
 * @brief Implementation of StringSet class
 */

#include "StringSet.hpp"
/**
 * @brief Union of two sets
 * @param other The second set
 * @return A new set that contains unique elements from both sets
 */
cse::StringSet cse::StringSet::Union(const StringSet& other) const {

  StringSet result;

  for (const auto& str : mElements) {
    result.insert(str);
  }

  for (const auto& str : other.mElements) {
    result.insert(str);
  }

  return result;
}

/**
 * @brief Intersection of two sets
 * @param other The second set
 * @return A set of common values in 2 sets
 */
cse::StringSet cse::StringSet::Intersection(const StringSet& other) const {

  StringSet result;

  for (const auto& str : mElements) {

    if (other.mElements.find(str) != other.mElements.end()) {
      result.insert(str);
    }
  }

  return result;
}

/**
 * @brief Operates differentiation between two string sets
 * @param other the second set
 * @return the difference set
 */
cse::StringSet cse::StringSet::Difference(const cse::StringSet& other) const {

  StringSet result;

  // Iterate through every element in mElements
  for (const auto& str : mElements) {

    if (other.mElements.find(str) == other.mElements.end()) {
      result.insert(str);
    }
  }

  return result;
}

/**
 * @brief Delete or keep elements based on filter and boolean parameter
 * @param filter Custom filter function
 * @param erase Boolean, RetainFilter function calls with false, RemoveFilter function calls with true
 */
void cse::StringSet::Filter(const std::function<bool(const std::string &)> filter, bool erase) {
    std::vector<std::string> filteredElements;

  for(const auto& str : mElements) {
    if(erase == filter(str)){
      filteredElements.push_back(str);
    }
  }

  for(const auto& str : filteredElements){
    mElements.erase(str);
  }
}

/**
 * @brief Keep elements in string set based on custom filter
 * @param filter custom function
 */
void cse::StringSet::RetainFilter(const std::function<bool(const std::string&)> filter) {
  Filter(filter, false);
}

/**
 * @brief Delete elements based on custom filter
 * @param filter custom function
 */
void cse::StringSet::RemoveFilter(const std::function<bool(const std::string&)> filter) {
  Filter(filter, true);
}

/**
 * @brief Checks if a current set is a subset of another set
 * 
 * @param other Other set that is a superset 
 * @return true Current set is a subset of other set
 * @return false Current set is not a subset of other set
 */
bool cse::StringSet::IsSubset(const StringSet &other) const{
  // Prepare a lambda that checks if captured element is in the other set
  // Used DeepSeek to assist with this lambda
  auto otherPredicate = [&other] (const std::string& element) {
    return other.count(element) == 1;
  };

  // use all_of in ranges to loop through each element in mElements
  return std::ranges::all_of(mElements, otherPredicate);
}

/**
 * @brief Returns a StringSet with words that contain a substring
 * 
 * @param substring Substring to check against
 */
void cse::StringSet::SubstringFilter(const std::string &substring) {
  // Lambda that will be used in RemoveFilter to filter words that we need
  auto substringPredicate = [&substring] (const std::string& element) {
    return element.find(substring) != std::string::npos;
  };

  RemoveFilter(substringPredicate);
}
cse::StringSet cse::StringSet::Search(const std::string &wildcard){
  StringSet result;
  return result;
}

int cse::StringSet::countOccurence(const std::string &substring){
  return 0;
}
cse::StringSet cse::StringSet::RandomSample(int sampleSize){
  StringSet result;
  return result;
}
std::tuple<std::tuple<int, int>, std::tuple<std::string, std::string>> cse::StringSet::Statistics(){
  return std::make_tuple(std::make_tuple(0, 0), std::make_tuple("", ""));
}
