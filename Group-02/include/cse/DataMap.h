/**
 * @file DataMap.h
 * @author Rahul Baragur
 */

#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <any>
#include <typeinfo>
#include <iostream>
#include <stdexcept>

namespace cse
{

/**
 * DataMap Class
 *
 * Inherited from std::unordered_map
 *
 */
class DataMap : public std::unordered_map<std::string, std::any>
{
public:
    template<typename T> void set(const std::string &name, const T &val) { (*this)[name] = val; }

    /**
     * Get the string key value from the DataMap
     * @tparam T value to be returned
     * @param name key to be looked up as an std::string
     * @return T the value
     */
    template<typename T> T get(const std::string &name)
    {
        if(contains(name))
        {
            const std::any &val = (*this)[name];
            if(val.type() == typeid(T))
            {
                return std::any_cast<T>((*this)[name]);
            }
            throw(std::runtime_error("Wrong type requested from what is contained within DataMap for key: " + name));
        }
        set(name, T{});
        return T{};
    }

    bool contains(const std::string &name);



};
}
