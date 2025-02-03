/**
 * @file Serializer.h
 * @author tdkduong
 */

#ifndef CSE_SERIALIZER_H
#define CSE_SERIALIZER_H

#include <iostream>
#include <sstream>
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
        // Constructor: accepts a mode and a reference to an iostream.
        Serializer(Mode mode, std::iostream &stream);

        // Generic Serialize method for simple (POD) types.
        template <typename T>
        void Serialize(T &data);

        // Special overload for std::string.
        void Serialize(std::string &str);

    private:
        Mode mode_;
        std::iostream &stream_;
    };
}

namespace cse
{
    template <typename T>
    void Serializer::Serialize(T &data)
    {
        if (mode_ == Mode::SAVE)
        {
            stream_.write(reinterpret_cast<const char *>(&data), sizeof(T));
        }
        else if (mode_ == Mode::LOAD)
        {
            stream_.read(reinterpret_cast<char *>(&data), sizeof(T));
        }
    }

} // namespace cse

#endif // CSE_SERIALIZER_H