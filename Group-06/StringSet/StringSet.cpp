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
cse::StringSet cse::StringSet::Union(const StringSet& other)const
{
    // Create new set result
    StringSet result;

    for(const auto& str : mElements)
    {
        // Add elements in mElements to result
        result.insert(str);
    }

    for(const auto& str : other.mElements)
    {
        // Add elements in other to result
        result.insert(str);
    }

    return result;
}


/**
 * @brief Intersection of two sets
 * @param other The second set
 * @return A set of common values in 2 sets
 */
cse::StringSet cse::StringSet::Intersection(const StringSet& other)const
{
    // Create new set result
    StringSet result;

    // Iterate through every element in mElements
    for(const auto& str : mElements)
    {
        // Check if we have str in other, if we don't .find
        // returns other.end() iterator, otherwise iterator pointing
        // to that element
        if(other.mElements.find(str) != other.mElements.end())
        {
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
cse::StringSet cse::StringSet::Difference(const cse::StringSet &other) const {
    // Create new set result
    StringSet result;

    // Iterate through every element in mElements
    for(const auto& str : mElements)
    {
        // Check if str not in other
        if(other.mElements.find(str) == other.mElements.end())
        {
            result.insert(str);
        }
    }

    return result;
}


/**
 * @brief Keep elements in string set based on custom filter
 * @param filter custom function
 */
void cse::StringSet::Filter(std::function<bool(const std::string &)> filter)
{
    std::vector<std::string> filteredElements;

    // Filter every element in mElements
    for (const auto& str : mElements)
    {
        if (!filter(str))
        {
            filteredElements.push_back(str);
        }
    }

    // Remove strings that do not meet the criteria
    for (const auto& str : filteredElements)
    {
        mElements.erase(str);
    }
}


/**
 * @brief Delete elements based on custom filter
 * @param filter custom function
 */
void cse::StringSet::FilterOut(std::function<bool(const std::string &)> filter)
{

    std::vector<std::string> filteredElements;

    // Filter every element in m_elements
    for (const auto& str : mElements)
    {
        if (filter(str))
        {
            filteredElements.push_back(str);
        }
    }

    // Remove filtered elements (strings that meet the criteria)
    for (const auto& str : filteredElements)
    {
        mElements.erase(str);
    }
}

