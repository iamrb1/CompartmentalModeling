/**
 * @file Serializer.h
 * @author tdkduong
 */

#ifndef CSE_SERIALIZER_H
#define CSE_SERIALIZER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <deque>
#include <stack>
#include <queue>
#include <string>

#include <type_traits>

namespace cse
{
    // The mode in which the serializer operates.
    enum class Mode
    {
        SAVE,
        LOAD
    };
    class Serializer
    {
    private:
        Mode mode_;

    public:
        // Constructor
        explicit Serializer() { mode_ = cse::Mode::SAVE; } // Set mode_ to SAVE by default
        explicit Serializer(Mode mode) : mode_(mode) {}
        ~Serializer() = default;
        // Checking current mode
        Mode GetMode()
        {
            return mode_;
        }
        bool IsSave() { return (mode_ == cse::Mode::SAVE); }
        bool IsLoad() { return (mode_ == cse::Mode::LOAD); }
        // Swapping Mode
        void SwapMode()
        {
            if (mode_ == cse::Mode::SAVE)
                mode_ = cse::Mode::LOAD;
            else if (mode_ == cse::Mode::LOAD)
                mode_ = cse::Mode::SAVE;
        }
        // Generic Serialize and Deserialze method for simple types
        template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
        void Serialize(T &data, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                outFile.write(reinterpret_cast<const char *>(&data), sizeof(T));
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                inFile.read(reinterpret_cast<char *>(&data), sizeof(T));
            }
        }

        // Serialize std::string
        void Serialize(std::string &data, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                size_t length = data.size();
                outFile.write(reinterpret_cast<const char *>(&length), sizeof(size_t));
                outFile.write(data.c_str(), length);
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                size_t length;
                inFile.read(reinterpret_cast<char *>(&length), sizeof(size_t));
                data.resize(length);
                inFile.read(&data[0], length);
            }
        }

        // Serialize std::vector
        template <typename T>
        void Serialize(std::vector<T> &vec, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                size_t size = vec.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                outFile.write(reinterpret_cast<const char *>(vec.data()), size * sizeof(T));
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                vec.resize(size);
                inFile.read(reinterpret_cast<char *>(vec.data()), size * sizeof(T));
            }
        }

        // Serialize std::array<T, N>
        template <typename T, std::size_t N>
        void Serialize(std::array<T, N> &arr, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                outFile.write(reinterpret_cast<const char *>(arr.data()), sizeof(T) * N);
            }
            else // Mode::LOAD
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                inFile.read(reinterpret_cast<char *>(arr.data()), sizeof(T) * N);
            }
        }

        // Serialize std::set<T>
        template <typename T>
        void Serialize(std::set<T> &set, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                size_t size = set.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : set)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
                    return;
                }
                set.clear();
                for (size_t i = 0; i < size; i++)
                {
                    T item;
                    inFile.read(reinterpret_cast<char *>(&item), sizeof(T));
                    if (!inFile)
                    {
                        std::cerr << "Error: Unable to read element " << i << " from file: " << filename << std::endl;
                        return;
                    }
                    set.insert(item);
                }
            }
        }

        // Serialize std::unordered_set<T>
        template <typename T>
        void Serialize(std::unordered_set<T> &uset, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                size_t size = uset.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : uset)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
                    return;
                }
                uset.clear();
                for (size_t i = 0; i < size; i++)
                {
                    T item;
                    inFile.read(reinterpret_cast<char *>(&item), sizeof(T));
                    if (!inFile)
                    {
                        std::cerr << "Error: Unable to read element " << i << " from file: " << filename << std::endl;
                        return;
                    }
                    uset.insert(item);
                }
            }
        }

        // Serialize std::multiset<T>
        template <typename T>
        void Serialize(std::multiset<T> &mset, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                size_t size = mset.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : mset)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
                    return;
                }
                mset.clear();
                for (size_t i = 0; i < size; i++)
                {
                    T item;
                    inFile.read(reinterpret_cast<char *>(&item), sizeof(T));
                    if (!inFile)
                    {
                        std::cerr << "Error: Unable to read element " << i << " from file: " << filename << std::endl;
                        return;
                    }
                    mset.insert(item);
                }
            }
        }

        // Serialize std::unordered_multiset<T>
        template <typename T>
        void Serialize(std::unordered_multiset<T> &umset, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    return;
                }
                size_t size = umset.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : umset)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                    return;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
                    return;
                }
                umset.clear();
                for (size_t i = 0; i < size; i++)
                {
                    T item;
                    inFile.read(reinterpret_cast<char *>(&item), sizeof(T));
                    if (!inFile)
                    {
                        std::cerr << "Error: Unable to read element " << i << " from file: " << filename << std::endl;
                        return;
                    }
                    umset.insert(item);
                }
            }
        }

        // Serialize std::map
        template <typename K, typename V>
        void Serialize(std::map<K, V> &map, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                }
                size_t size = map.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const auto &[key, value] : map)
                {
                    outFile.write(reinterpret_cast<const char *>(&key), sizeof(K));
                    outFile.write(reinterpret_cast<const char *>(&value), sizeof(V));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error reading map size from file: " << filename << std::endl;
                }
                map.clear();
                for (size_t i = 0; i < size; i++)
                {
                    K key;
                    V value;
                    inFile.read(reinterpret_cast<char *>(&key), sizeof(K));
                    inFile.read(reinterpret_cast<char *>(&value), sizeof(V));

                    if (!inFile)
                    {
                        std::cerr << "Error reading element " << i << " from file: " << filename << std::endl;
                    }
                    map[key] = value;
                }
            }
        }

        // Serialize std::unordered_map
        template <typename K, typename V>
        void Serialize(std::unordered_map<K, V> &umap, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                }
                size_t size = umap.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const auto &[key, value] : umap)
                {
                    outFile.write(reinterpret_cast<const char *>(&key), sizeof(K));
                    outFile.write(reinterpret_cast<const char *>(&value), sizeof(V));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error reading map size from file: " << filename << std::endl;
                }
                umap.clear();
                for (size_t i = 0; i < size; i++)
                {
                    K key;
                    V value;
                    inFile.read(reinterpret_cast<char *>(&key), sizeof(K));
                    inFile.read(reinterpret_cast<char *>(&value), sizeof(V));

                    if (!inFile)
                    {
                        std::cerr << "Error reading element " << i << " from file: " << filename << std::endl;
                    }
                    umap[key] = value;
                }
            }
        }

        // Serialize std::multimap
        template <typename K, typename V>
        void Serialize(std::multimap<K, V> &mmap, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                }
                size_t size = mmap.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const auto &[key, value] : mmap)
                {
                    outFile.write(reinterpret_cast<const char *>(&key), sizeof(K));
                    outFile.write(reinterpret_cast<const char *>(&value), sizeof(V));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error reading map size from file: " << filename << std::endl;
                }
                mmap.clear();
                for (size_t i = 0; i < size; i++)
                {
                    K key;
                    V value;
                    inFile.read(reinterpret_cast<char *>(&key), sizeof(K));
                    inFile.read(reinterpret_cast<char *>(&value), sizeof(V));

                    if (!inFile)
                    {
                        std::cerr << "Error reading element " << i << " from file: " << filename << std::endl;
                    }
                    mmap.insert({key, value});
                }
            }
        }

        // Serialize std::unordered_multimap
        template <typename K, typename V>
        void Serialize(std::unordered_multimap<K, V> &ummap, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                }
                size_t size = ummap.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const auto &[key, value] : ummap)
                {
                    outFile.write(reinterpret_cast<const char *>(&key), sizeof(K));
                    outFile.write(reinterpret_cast<const char *>(&value), sizeof(V));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error reading map size from file: " << filename << std::endl;
                }
                ummap.clear();
                for (size_t i = 0; i < size; i++)
                {
                    K key;
                    V value;
                    inFile.read(reinterpret_cast<char *>(&key), sizeof(K));
                    inFile.read(reinterpret_cast<char *>(&value), sizeof(V));

                    if (!inFile)
                    {
                        std::cerr << "Error reading element " << i << " from file: " << filename << std::endl;
                    }
                    ummap.insert({key, value});
                }
            }
        }

        // Serialize stack
        template <typename T>
        void Serialize(std::stack<T> &stk, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                }
                std::vector<T> vec;
                std::stack<T> temp = stk;
                while (!temp.empty())
                {
                    vec.push_back(temp.top());
                    temp.pop();
                }
                size_t size = vec.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : vec)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error reading stack size from file: " << filename << std::endl;
                }
                std::vector<T> vec(size);
                inFile.read(reinterpret_cast<char *>(vec.data()), sizeof(T) * size);
                stk = std::stack<T>();
                for (auto it = vec.rbegin(); it != vec.rend(); ++it)
                {
                    stk.push(*it);
                }
            }
        }

        // // Serialize queue
        template <typename T>
        void Serialize(std::queue<T> &q, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    std::abort();
                }
                std::vector<T> vec;
                std::queue<T> temp = q;
                while (!temp.empty())
                {
                    vec.push_back(temp.front());
                    temp.pop();
                }
                size_t size = vec.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : vec)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));

                if (!inFile)
                {
                    std::cerr << "Error reading queue size from file: " << filename << std::endl;
                }
                std::vector<T> vec(size);
                inFile.read(reinterpret_cast<char *>(vec.data()), sizeof(T) * size);
                q = std::queue<T>();
                for (const T &item : vec)
                {
                    q.push(item);
                }
            }
        }

        // Serialize priority_queue
        template <typename T>
        void Serialize(std::priority_queue<T> &pq, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    std::abort();
                }
                std::vector<T> vec;
                std::priority_queue<T> temp = pq;
                while (!temp.empty())
                {
                    vec.push_back(temp.top());
                    temp.pop();
                }
                size_t size = vec.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : vec)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile)
                {
                    std::cerr << "Error reading priority queue size from file: " << filename << std::endl;
                }
                std::vector<T> vec(size);
                inFile.read(reinterpret_cast<char *>(vec.data()), sizeof(T) * size);
                pq = std::priority_queue<T>(vec.begin(), vec.end());
            }
        }

        // Serialize Deque
        template <typename T>
        void Serialize(std::deque<T> &deq, const std::string &filename)
        {
            if (mode_ == Mode::SAVE)
            {
                std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
                if (!outFile)
                {
                    std::cerr << "Error opening file for writing: " << filename << std::endl;
                    std::abort();
                }
                size_t size = deq.size();
                outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
                for (const T &item : deq)
                {
                    outFile.write(reinterpret_cast<const char *>(&item), sizeof(T));
                }
            }
            else
            {
                std::ifstream inFile(filename, std::ios::binary);
                if (!inFile)
                {
                    std::cerr << "Error opening file for reading: " << filename << std::endl;
                }
                size_t size;
                inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
                if (!inFile || inFile.eof())
                {
                    std::cerr << "Error reading deque size from file: " << filename << std::endl;
                    std::abort();
                }
                deq.clear();
                for (size_t i = 0; i < size; i++)
                {
                    T item;
                    inFile.read(reinterpret_cast<char *>(&item), sizeof(T));
                    if (!inFile)
                    {
                        std::cerr << "Error reading element " << i << " from file: " << filename << std::endl;
                    }
                    deq.push_back(item);
                }
            }
        }

        // Specialized function for serializing user-defined classes/structs (Currently not in use)
        template <typename T, typename std::enable_if_t<std::is_class_v<T> && !std::is_same_v<T, std::string>, int> = 0>
        void Serialize(T &obj, const std::string &filename)
        {
            obj.Serialize(*this, filename);
        }
    };
} // namespace cse

#endif // CSE_SERIALIZER_H