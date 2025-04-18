/**
 * @file MockImage.hpp
 *
 * @author Mia Bodenhorn
 */

#pragma once


#include <cstdlib>
#include <string>

/**
 * @brief A struct to represent an image
 */
class MockImage {
 public:
  MockImage(const std::string& url, int width, int height,
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


  /** @return std::string unique ID for MockImage */
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


