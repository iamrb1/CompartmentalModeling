/**
 * @file image.hpp
 *
 * @author Mia Bodenhorn
 */

#pragma once

#include <emscripten.h>

#include <cstdlib>
#include <string>

namespace cse {
/**
 * @brief A struct to represent an image
 */
class Image {
 public:
  Image(const std::string& url, int width, int height,
        const std::string& altText = "", const std::string imageID = "");

  /** @return The url of the image */
  std::string getURL() const { return url; }
  /** @return The width of the image */
  int getWidth() const { return width; }
  /** @return The height of the image */
  int getHeight() const { return height; }
  /** @return The alternative text of the image */
  std::string getAltText() const { return altText; }
  /** @return The unique id of the image */
  std::string getID() const { return id; }

  void setURL(const std::string& newURL);
  void resize(int newWidth, int newHeight, bool maintainAspect = true);
  void updateAltText(const std::string& newAltText);

  std::string renderHTML() const;
  std::string generateJS() const;

  void injectJS() const;

  void preview() const;

  /** @return std::string unique ID for Image */
  static std::string generateID() {
    static int counter = 1;
    return "image-" + std::to_string(counter++);
  }

 private:
  std::string url;
  int width;
  int height;
  std::string altText;
  std::string id;
  // bool validateURL(const std::string& url) const;
};

void Alert(const std::string& msg);

#ifdef NDEBUG
#define em_assert(...)
#else
#define em_assert(condition)                                              \
  do {                                                                    \
    if (!(condition)) {                                                   \
      std::string msg =                                                   \
          "Assertion failed: " #condition "\nFile: " __FILE__ "\nLine: "; \
      msg += std::to_string(__LINE__);                                    \
      Alert(msg);                                                         \
      std::abort();                                                       \
    }                                                                     \
  } while (0)
#endif

}  // namespace cse
