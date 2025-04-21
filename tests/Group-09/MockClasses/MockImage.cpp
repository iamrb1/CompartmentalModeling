/**
 * @file Image.cpp
 *
 * @author Mia Bodenhorn
 */

#include "MockImage.hpp"

#include <assert.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace cse {
void Alert(const std::string& msg) {}

/**
 * @brief Validates whether URL starts with http or https
 *
 * @param url the URL to validate
 * @return True if the URL is valid
 */
bool validateURL(const std::string& url) {
  return url.find("http://") == 0 ||
         url.find("https://") == 0 ||
         url.find("data:") == 0 ||
         url.find("://") == std::string::npos;
}

/**
 * @brief Constructs an Image object with specified URL, dimensions, and
 * alternative text.
 *
 * @param url The image url
 * @param width The image width in pixels
 * @param height The image height in pixels
 * @param altText The alternative text for image
 * @throws std::invalid_argument If url is invalid or dimensions aren't positive
 */
MockImage::MockImage(const std::string& url, int width, int height,
             const std::string& altText, const std::string imageID)
    : url(url),
      width(width),
      height(height),
      altText(altText),
      id(imageID.empty() ? generateID() : imageID) {
  if (url.empty()) {
    throw std::invalid_argument("URL must not be empty");
  }
  if (!validateURL(url)) {
    throw std::invalid_argument(
        "Invalid URL: Must be valid http/https URL or a relative path");
  }
  if (width <= 0 || height <= 0) {
    throw std::invalid_argument(
        "Invalid dimensions: Width and height must be positive integers");
  }
}

/**
 * @brief Sets a new url for the image
 *
 * @param newURL The new image url
 * @throws std::invalid_argument If the URL is invalid
 */
void MockImage::setURL(const std::string& newURL) {
  if (newURL.empty()) {
    throw std::invalid_argument("New URL must not be empty");
  }
  if (!validateURL(newURL)) {
    throw std::invalid_argument(
        "Invalid URL: Must start with http:// or https://");
  }
  url = newURL;
}

/**
 * @brief Resizes the image with an option to maintain aspect ratio
 *
 * @param newWidth New width of image
 * @param newHeight New height of image
 * @param maintainAspect If true, maintains the original ratio
 * @throws std::invalid_argument If new dimensions aren' t positive
 */
void MockImage::resize(int newWidth, int newHeight, bool maintainAspect) {
  if (newWidth <= 0 || newHeight <= 0) {
    throw std::invalid_argument(
        "Invalid dimensions: Width and height must be positive.");
  }
  if (maintainAspect) {
    double aspectRatio = static_cast<double>(width) / height;
    if (newWidth / aspectRatio > newHeight) {
      newWidth = static_cast<int>(newHeight * aspectRatio);
    } else {
      newHeight = static_cast<int>(newWidth / aspectRatio);
    }
  }
  width = newWidth;
  height = newHeight;
}

/**
 * @brief Updates the alternative text for the image
 *
 * @param newAltText The new text
 */
void MockImage::updateAltText(const std::string& newAltText) {
  altText = newAltText;
}

/** @return A string containing the HTML representation of the image */
std::string MockImage::renderHTML() const {
  std::ostringstream html;
  html << "<img src=\"" << url << "\" width=\"" << width << "\" height=\""
       << height << "\" alt=\"" << altText << "\">";
  return html.str();
}

/** @return A string containing JavaScript code to dynamically insert image into
 * webpage */
std::string MockImage::generateJS() const {
  std::ostringstream js;
  js << "var img = document.createElement('img');\n"
     << "img.src = '" << url << "';\n"
     << "img.width = " << width << ";\n"
     << "img.height = " << height << ";\n"
     << "img.alt = '" << altText << "';\n"
     << "document.body.appendChild(img);";
  return js.str();
}

/**
 * @brief Logs a preview of the image details for debugging purposes.
 *
 * This function prints image attributes such as URL, dimensions, and alt text,
 * along with the generated HTML and JavaScript, to the browser console.
 *
 */
void MockImage::preview() const {
  std::ostringstream output;
  output << "Image Preview:\n"
         << "URL: " << url << "\n"
         << "Dimensions: " << width << "x" << height << "\n"
         << "Alt Text: " << altText << "\n"
         << "HTML: " << renderHTML() << "\n"
         << "JavaScript: " << generateJS() << "\n";

}

/**
 * @brief Injects the image into the HTML document using JavaScript
 *
 *
 * The image is appended to the document body
 */

void MockImage::injectJS() const {

}

}  // namespace cse
