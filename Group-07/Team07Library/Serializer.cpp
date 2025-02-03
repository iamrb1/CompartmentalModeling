#include "Serializer.h"

namespace cse
{
    Serializer::Serializer(Mode mode, std::iostream &stream)
        : mode_(mode), stream_(stream) {}

    void Serializer::Serialize(std::string &str)
    {
        if (mode_ == Mode::SAVE)
        {
            // Write the length of the string.
            size_t size = str.size();
            Serialize(size); // Calls the templated version.
            // Write the string characters.
            stream_.write(str.data(), size);
        }
        else if (mode_ == Mode::LOAD)
        {
            // Read the length of the string.
            size_t size;
            Serialize(size);
            // Resize and then read the string characters.
            str.resize(size);
            stream_.read(&str[0], size);
        }
    }

} // namespace cse
