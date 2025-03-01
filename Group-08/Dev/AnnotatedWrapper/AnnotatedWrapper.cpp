#include "AnnotatedWrapper.h"

/**
 * @brief Adds an annotation with a key-value pair.
 * @param key The annotation key.
 * @param value The annotation value.
 */
void cse::AnnotatedWrapper::addAnnotation(const std::string& key, const std::string& value) {
    mAnnotations[key] = value;
}

/**
 * @brief Retrieves an annotation by key.
 * @param key The key to look for.
 * @return The value associated with the key, or an empty string if not found.
 */
std::string cse::AnnotatedWrapper::getAnnotation(const std::string key) {
    return mAnnotations.at(key);
}

/**
 * @brief Removes an annotation by key.
 * @param key The key to remove.
 */
void cse::AnnotatedWrapper::removeAnnotation(const std::string key) {
    mAnnotations.erase(key);
}

/**
 * @brief Lists all stored annotations.
 */
void cse::AnnotatedWrapper::listAnnotations() {
    if (mAnnotations.empty()) {
        std::cout << "No annotations available." << std::endl;
        return;
    }

    for (const auto& pair : mAnnotations) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

/**
 * @brief Clears all stored annotations.
 */
void cse::AnnotatedWrapper::clearAnnotations() {
    mAnnotations.clear();
}