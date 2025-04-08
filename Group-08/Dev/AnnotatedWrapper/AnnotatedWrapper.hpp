#ifndef ANNOTATEDWRAPPER_H
#define ANNOTATEDWRAPPER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>

namespace cse {

    /**
     * @class   AnnotatedWrapper
     * @file    AnnotatedWrapper.hpp
     * @author  Joanna
     * @brief   The Annotated Wrapper class is "a base class that provides 
     *          “annotating” features to any derived class. A simple form of 
     *          annotations would be tags associated with the class."
     */
    
    // A template class that stores annotations and visual styling.
    template<typename T>
    class AnnotatedWrapper {
    private:
        // A map to hold annotation key-value pairs.
        std::unordered_map<std::string, T> mAnnotations;

        // Visual styling properties
        int mFontSize = 0;

    public:
        /**
         * Adds or updates an annotation with the specified key and value
         * 
         * @param key
         * @param value
         */
        void addAnnotation(const std::string& key, const T& value) {
            mAnnotations[key] = value;
        }

        /**
         * Retrieves the annotation for a given key
         * 
         * @param key
         * @return default-constructed T if the key is not found
         */
        T getAnnotation(const std::string& key) const {
            auto it = mAnnotations.find(key);
            return (it != mAnnotations.end()) ? it->second : T{};
        }

        /**
         * Removes an annotation by its key
         */
        void removeAnnotation(const std::string& key) {
            mAnnotations.erase(key);
        }

        /**
         * Removes all annotations from the map
         */
        void clearAnnotations() {
            mAnnotations.clear();
        }

        /**
         * Iterates through all annotations and applies a callback
         * 
         * @param string&
         * @param callback 
         */
        void listAnnotations(std::function<void(const std::string&, const T&)> callback) const {
            for (const auto& [key, value] : mAnnotations) {
                callback(key, value);
            }
        }

        /**
         * Sets the font size
         * 
         * @param size size that the text will be set to
         */
        void setFontSize(int size) {
            mFontSize = size;
        }

        /**
         * Rereives the font size
         */
        int getFontSize() const {
            return mFontSize;
        }

    };
}

#endif // ANNOTATEDWRAPPER_H