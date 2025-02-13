/**
 * @file Serializer.h
 * @author tdkduong
 */

#ifndef CSE_SERIALIZER_H
#define CSE_SERIALIZER_H

#include <iostream>
#include <fstream>
#include <string>

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
    public:
        // Constructor
        explicit Serializer(Mode mode) : mode_(mode) {}

        // Generic Serialize and Deserialze method for simple types, based on current mode
        template <typename T>
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

    private:
        Mode mode_;
    };
} // namespace cse

#endif // CSE_SERIALIZER_H