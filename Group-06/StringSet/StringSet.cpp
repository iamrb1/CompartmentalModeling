/**
 * Implementation of StringSet class
 *
 * created by Orhan Aydin
 */

#include "StringSet.h"

/**
 * add str into the set named m_elements
 * @param str
 */
void cse::StringSet::insert(const std::string& str)
{
    m_elements.insert(str);
}


/**
 * erase str from m_elements
 * @param str
 */
void cse::StringSet::erase(const std::string &str)
{
    m_elements.erase(str);
}

/**
 * make m_elements empty
 */
void cse::StringSet::clear()
{
    m_elements.clear();
}


/**
 * return size of m_elements
 * @return
 */
size_t cse::StringSet::size() const
{
    return m_elements.size();
}

/**
 * check if m_elements is empty
 * @return
 */
bool cse::StringSet::empty() const
{
    return m_elements.empty();
}


/**
 * return a new set that contains unique elements from both sets
 * @param other
 * @return
 */
cse::StringSet cse::StringSet::Union(const StringSet& other)const
{
    StringSet result; //create new set result

    for(const auto& str : m_elements)
    {
        result.insert(str); // add elements in m_elements to result
    }

    for(const auto& str : other.m_elements)
    {
        result.insert(str); // add elements in other to result
    }

    return result; // return result
}


/**
 * return a set of common values in 2 sets
 * @param other
 * @return
 */
cse::StringSet cse::StringSet::Intersection(const StringSet& other)const
{
    StringSet result; // new set result

    for(const auto& str : m_elements) // iterate through every element in m_elements
    {
        if(other.m_elements.find(str) != other.m_elements.end()) // check if we have str in other, if we don't .find
            // returns other.end() iterator, otherwise iterator pointing
            //to that element
        {
            result.insert(str);
        }
    }

    return result;
}