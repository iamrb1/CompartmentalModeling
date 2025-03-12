/**
 * @file StringSet.cpp
 * @author Orhan Aydin 
 * @author Emil Rahn-Siegel
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
/**
 * @brief Search for a string with wildcard
 * 
 * @param wildcard Wildcard string
 * @return StringSet Set of strings that match the wildcard
 */
cse::StringSet cse::StringSet::Search(const std::string &wildcard) {
    StringSet result;
    // Using std::regex_replace was recommended to me by github co
    // Convert wildcard to regex
    std::string regexPattern = "^" + std::regex_replace(wildcard, std::regex(R"(\*)"), ".*") + "$";
    regexPattern = std::regex_replace(regexPattern, std::regex(R"(\?)"), ".");

    // Create regex object
    std::regex pattern(regexPattern);

    // Iterate through elements and add matching ones to result
    for (const auto& element : mElements) {
        if (std::regex_match(element, pattern)) {
            result.insert(element);
        }
    }

    return result;
}

int cse::StringSet::countOccurence(const std::string &substring) {
  int totalCount = 0;

  for (const auto& element : mElements) {
      size_t pos = element.find(substring);
      while (pos != std::string::npos) {
          ++totalCount;
          pos = element.find(substring, pos + substring.length());
      }
  }

  return totalCount;
}
cse::StringSet cse::StringSet::RandomSample(int sampleSize) {
  StringSet result;
  if (sampleSize <= 0 || static_cast<std::unordered_set<std::string>::size_type>(sampleSize) > mElements.size()) {
      return result;
  }

  std::vector<std::string> elements(mElements.begin(), mElements.end());
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(elements.begin(), elements.end(), gen);

  for (int i = 0; i < sampleSize; ++i) {
      result.insert(elements[i]);
  }

  return result;
}
std::tuple<int, double, std::string, std::string> cse::StringSet::Statistics() {
    int numStrings = size();
    if (numStrings == 0) {
        return std::make_tuple(0, 0.0, "", "");
    }

    int totalLength = 0;
    std::string longestString = "";
    std::string shortestString = *mElements.begin();
    
    for (const auto& str : mElements) {
        int length = static_cast<int>(str.length());
        totalLength += length;
        if (length > static_cast<int>(longestString.length())) {
            longestString = str;
        }
        if (length < static_cast<int>(shortestString.length())) {
            shortestString = str;
        }
    }

    double avgLength = static_cast<double>(totalLength) / numStrings;

    return std::make_tuple(numStrings, avgLength, longestString, shortestString);
}

/**
 * @brief Removes strings of length not equal to size
 * 
 * @param size length of strings that will be retained
 */
void cse::StringSet::SizeFilter(int length) {
  assert(length > 0);
  
  auto sizePredicate = [length] (const std::string& element) {
    return element.size() == static_cast<std::size_t>(length);
  };

  RetainFilter(sizePredicate);
}
