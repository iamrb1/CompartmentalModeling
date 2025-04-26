/**
* @file image.hpp
 *
 * @author Mia Bodenhorn
 */

#pragma once

#include <string>
#include <emscripten.h>
#include <cstdlib>

namespace cse {
/**
 * @brief A struct to represent an image
 */
class Image {
 public:
  Image(const std::string& url, int width, int height, const std::string& altText = "");

  /** @return The url of the image */
  std::string getURL() const { return url; }
  /** @return The width of the image */
  int getWidth() const { return width; }
  /** @return The height of the image */
  int getHeight() const { return height; }
  /** @return The alternative text of the image */
  std::string getAltText() const { return altText; }

  void setURL(const std::string& newURL);
  void resize(int newWidth, int newHeight, bool maintainAspect = true);
  void updateAltText(const std::string& newAltText);

  std::string renderHTML() const;
  std::string generateJS() const;

  void injectJS() const;

  void preview() const;

 private:
  std::string url;
  int width;
  int height;
  std::string altText;
  // bool validateURL(const std::string& url) const;

};

void Alert(const std::string& msg);

#ifdef NDEBUG
  #define em_assert(...)
#else
  #define em_assert(condition)                            \
    do {                                                  \
      if (!(condition)) {                                 \
        std::string msg = "Assertion failed: " #condition \
          "\nFile: " __FILE__ "\nLine: ";                 \
        msg += std::to_string(__LINE__);                  \
        Alert(msg);                                       \
        std::abort();                                     \
      }                                                   \
    } while (0)
#endif


} // namespace cse
