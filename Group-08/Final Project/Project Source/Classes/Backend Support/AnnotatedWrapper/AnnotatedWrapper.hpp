#ifndef ANNOTATEDWRAPPER_H
#define ANNOTATEDWRAPPER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>
#include <optional>

namespace cse {

    /**
     * @class   AnnotatedWrapper
     * @file    AnnotatedWrapper.hpp
     * @author  Joanna Rodriguez Zamarron
     * @brief   A generic base class that provides annotation and visual styling capabilities
     *          to derived classes. Useful for tagging, labeling, or storing extra metadata
     *          on an object via key-value pairs.
     * 
     * @tparam T The type of values associated with annotation keys.
     *
     * Annotations are stored as key-value pairs, where:
     * - `key` is a unique string identifier (e.g., "color", "priority")
     * - `value` is the associated metadata of type `T`
     */
    template<typename T>
    class AnnotatedWrapper {
    private:
        /**
         * @brief Stores annotation key-value pairs.
         * The key is a string representing the name of the annotation (e.g., "color"),
         * and the value is of type T (e.g., "red" or 3.14).
         */
        std::unordered_map<std::string, T> mAnnotations;

        /// Font size for optional visual styling (not tied to annotations)
        int mFontSize = 0;

    public:
        /**
         * @brief Adds or updates an annotation.
         * 
         * @param key A unique name identifying the annotation (e.g., "category", "author")
         * @param value The metadata value associated with the key
         */
        void addAnnotation(const std::string& key, const T& value) {
            mAnnotations[key] = value;
        }

        /**
         * @brief Retrieves the value associated with a given annotation key.
         * 
         * @param key The name of the annotation to retrieve
         * @return The annotation's value if found, otherwise a default-constructed T
         */
        T getAnnotation(const std::string& key) const {
            auto it = mAnnotations.find(key);
            return (it != mAnnotations.end()) ? it->second : T{};
        }

        /**
         * @brief Removes a single annotation by key.
         * 
         * @param key The name of the annotation to remove
         */
        void removeAnnotation(const std::string& key) {
            mAnnotations.erase(key);
        }

        /**
         * @brief Clears all stored annotations.
         */
        void clearAnnotations() {
            mAnnotations.clear();
        }

        /**
         * @brief Applies a callback function to all key-value annotation pairs.
         * 
         * @param callback A function that takes a (key, value) pair and performs an operation
         */
        void listAnnotations(std::function<void(const std::string&, const T&)> callback) const {
            for (const auto& [key, value] : mAnnotations) {
                callback(key, value);
            }
        }

        /**
         * @brief Sets the font size (used for optional display styling).
         * 
         * @param size An integer representing the desired font size
         */
        void setFontSize(int size) {
            mFontSize = size;
        }

        /**
         * @brief Retrieves the current font size setting.
         * 
         * @return The font size associated with the object
         */
        int getFontSize() const {
            return mFontSize;
        }

    };
}

#endif // ANNOTATEDWRAPPER_H