#ifndef ANNOTATEDWRAPPER_H
#define ANNOTATEDWRAPPER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>

namespace cse {

    template<typename T>
    class AnnotatedWrapper {
    private:
        std::unordered_map<std::string, T> mAnnotations;

        int mFontSize = 0;
        int mFontColor = 0;
        int mBackgroundColor = 0;

    public:
        void addAnnotation(const std::string& key, const T& value) {
            mAnnotations[key] = value;
        }

        T getAnnotation(const std::string& key) const {
            auto it = mAnnotations.find(key);
            return (it != mAnnotations.end()) ? it->second : T{};
        }

        void removeAnnotation(const std::string& key) {
            mAnnotations.erase(key);
        }

        void clearAnnotations() {
            mAnnotations.clear();
        }

        // template class that loops through all annotations in map and uses a callback function to run the key-value pair
        // callback is a function you pass to another function to be called later when ready
        void listAnnotations(std::function<void(const std::string&, const T&)> callback) const {
            for (const auto& [key, value] : mAnnotations) {
                callback(key, value);
            }
        }

        void setFontSize(int size) {
            mFontSize = size;
        }

        int getFontSize() const {
            return mFontSize;
        }

        void setFontColor(int color) {
            mFontColor = color;
        }

        int getFontColor() const {
            return mFontColor;
        }

        void setBackgroundColor(int color) {
            mBackgroundColor = color;
        }

        int getBackgroundColor() const {
            return mBackgroundColor;
        }
    };
}

#endif // ANNOTATEDWRAPPER_H