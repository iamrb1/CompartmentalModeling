#ifndef ANNOTATEDWRAPPER_H
#define ANNOTATEDWRAPPER_H

<<<<<<< HEAD
#include <iostream>
#include <unordered_map>
#include <any>


 /**
 * @class AnnotatedWrapper
 * @brief A wrapper class for managing key-value annotations.
 *
 * This class provides an easy-to-use interface for adding, retrieving,
 * and removing key-value annotations. It is designed to be lightweight 
 * and efficient using `std::unordered_map` for fast lookups.
 *
 * Internally:
 * - Uses `std::unordered_map<std::string, std::string>` to store 
 *   annotations as key-value pairs.
 *
 */
namespace cse {

    class AnnotatedWrapper {
    public:
        // Constructor
        AnnotatedWrapper(){
            mAnnotations = std::unordered_map<std::string, std::string>();
        };
        // Destructor
        // ~AnnotatedWrapper() = default;

        // Public methods
        void addAnnotation( const std::string& key, const std::string& value);

        // Temporary hard coded annotation types for testing only
        void addAnnotation( const std::string& wrapperKey, const std::string& personalKey, const std::string& value);
        void addAnnotation( const std::string& key, const float number);

        std::string getAnnotation(const std::string key);
        void removeAnnotation(const std::string key);
        void listAnnotations();
        void clearAnnotations();

        void setFontSize(float size);
        float getFontSize();

        void setFontColor(const std::string& color);
        const std::string& getFontColor();

        void setBackgroundColor (const std::string& color);
        const std::string& getBackgroundColor();

        int getAnnotationCount();

    private:
        std::unordered_map<std::string, std::string> mAnnotations;

        int mFontSize;
        int mFontColor;
        int mBackgroundColor;

=======
#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>

namespace cse {

    // A template class that stores annotations and visual styling.
    template<typename T>
    class AnnotatedWrapper {
    private:
        // A map to hold annotation key-value pairs.
        std::unordered_map<std::string, T> mAnnotations;

        // Visual styling properties
        int mFontSize = 0;
        int mFontColor = 0;
        int mBackgroundColor = 0;

    public:
        // Adds or updates an annotation with the specified key and value.
        void addAnnotation(const std::string& key, const T& value) {
            mAnnotations[key] = value;
        }

        // Retrieves the annotation for a given key.
        // Returns default-constructed T if the key is not found.
        T getAnnotation(const std::string& key) const {
            auto it = mAnnotations.find(key);
            return (it != mAnnotations.end()) ? it->second : T{};
        }

        // Removes an annotation by its key.
        void removeAnnotation(const std::string& key) {
            mAnnotations.erase(key);
        }

        // Clears all annotations from the map
        void clearAnnotations() {
            mAnnotations.clear();
        }

        // Iterates through all annotations and applies a callback
        void listAnnotations(std::function<void(const std::string&, const T&)> callback) const {
            for (const auto& [key, value] : mAnnotations) {
                callback(key, value);
            }
        }

        // Sets the font size 
        void setFontSize(int size) {
            mFontSize = size;
        }

        // Retrieves the current font size
        int getFontSize() const {
            return mFontSize;
        }

        // Sets the font color
        void setFontColor(int color) {
            mFontColor = color;
        }

        // Retrieves the current font color
        int getFontColor() const {
            return mFontColor;
        }

        // Sets the background color
        void setBackgroundColor(int color) {
            mBackgroundColor = color;
        }

        // Retrieves the current background color
        int getBackgroundColor() const {
            return mBackgroundColor;
        }
>>>>>>> 11b16d6c263a71a2ef9ac0b7df019ccc695a6518
    };
}

#endif // ANNOTATEDWRAPPER_H