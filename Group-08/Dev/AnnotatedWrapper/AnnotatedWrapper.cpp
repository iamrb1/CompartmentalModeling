#include "AnnotatedWrapper.h"

/**
 * @brief Adds an annotation with a key-value pair.
 * @param key The annotation key.
 * @param value The annotation value.
 */
void AnnotatedWrapper::addAnnotation(const std::string& key, const std::string& value) {
    annotations[key] = value;
}

/**
 * @brief Retrieves an annotation by key.
 * @param key The key to look for.
 * @return The value associated with the key, or an empty string if not found.
 */
std::string AnnotatedWrapper::getAnnotation(const std::string& key) {
    return annotations.at(key);
}

/**
 * @brief Removes an annotation by key.
 * @param key The key to remove.
 */
void AnnotatedWrapper::removeAnnotation(const std::string& key) {
    annotations.erase(key);
}

/**
 * @brief Lists all stored annotations.
 */
void AnnotatedWrapper::listAnnotations() const {
    if (annotations.empty()) {
        std::cout << "No annotations available." << std::endl;
        return;
    }

    for (const auto& pair : annotations) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

/**
 * @brief Clears all stored annotations.
 */
void AnnotatedWrapper::clearAnnotations() {
    annotations.clear();
}