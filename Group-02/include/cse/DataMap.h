/**
 * @file DataMap.h
 * @author Rahul Baragur
 */

#pragma once

#include <map>
#include <string>
#include <any>
#include <typeinfo>
#include <iostream>

namespace cse
{

/**
 * DataMap Class
 *
 */
class DataMap
{
private:
    std::unordered_map<std::string, std::any> m_map;

public:
    template<typename T> void set(const std::string &name, const T &val) { m_map[name] = val; }

    /**
     * Get the string key value from the DataMap
     * @tparam T value to be returned
     * @param name key to be looked up as an std::string
     * @return T the value
     */
    template<typename T> T get(const std::string &name)
    {
        if(keyContain(name))
        {
            const std::any &val = m_map[name];
            if(val.type() == typeid(T))
            {
                return std::any_cast<T>(m_map[name]);
            }
            std::cerr << "Wrong Type Provided by User in DataMap.get()" << std::endl;
            return T{};
        }
        set(name, T{});
        return T{};
    }

    bool keyContain(const std::string &name);

    void delKey(const std::string &name);

    /**
     * Clear the DataMap
     */
    void clear() { m_map.clear(); }

    /**
     * Gives the size of the map
     * @return unsigned long map size
     */
    unsigned long size() { return m_map.size(); }

    /**
     * Emptys the DataMap
     */
    int empty() { return m_map.empty(); }

    /**
     * Returns the count of the keys within the map
     * @param name key to be found
     * @return unsigned long # of keys
     */
    unsigned long count(const std::string &name) { return m_map.count(name); }

};
}
