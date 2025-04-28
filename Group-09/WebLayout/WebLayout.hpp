/**
 * Holds various text boxes and images to create "slides"
 *
 * @file WebLayout.h
 * @author Mary Holt, Grace Fitzgerald
 *
 */

#pragma once
#include <emscripten.h>

#include <iostream>
#include <memory>
#include <vector>

#include "../Image/Image.hpp"
#include "../TextBox/TextBox.hpp"

namespace cse {

struct TextBoxLayout {
  std::shared_ptr<TextBox> textBox;
  int yPos;
  int xPos;

  TextBoxLayout() : textBox(), xPos(0), yPos(0){};

  TextBoxLayout(std::shared_ptr<TextBox> textBox, int x, int y)
      : textBox(std::move(textBox)), xPos(x), yPos(y) {}

  bool operator==(const TextBoxLayout &textboxLayout) const {
    return ((textboxLayout.textBox->getFormattedText().getText() ==
             textBox->getFormattedText().getText()) &&
            (textboxLayout.textBox->getWidth() == textBox->getWidth()) &&
            (textboxLayout.textBox->getHeight() == textBox->getHeight()));
  }

  void setPosition(int x, int y) {
    xPos = x;
    yPos = y;
  }

};

struct ImageLayout {
  std::shared_ptr<Image> image;
  int yPos;
  int xPos;

  ImageLayout() : image(), yPos(0), xPos(0){};

  ImageLayout(std::shared_ptr<Image> image, int x, int y)
      : image(std::move(image)), yPos(y), xPos(x) {}

  bool operator==(const ImageLayout &imageLayout) const {
    return ((imageLayout.image->getURL() == image->getURL()) &&
            (imageLayout.image->getWidth() == image->getWidth()) &&
            (imageLayout.image->getHeight() == image->getHeight()));
  }

    void setPosition(int x, int y) {
    xPos = x;
    yPos = y;
  }
};

class WebLayout {
 private:
  std::vector<TextBoxLayout> textBoxes;
  std::vector<ImageLayout> images;
  std::string id;

  const void renderTextBox(const std::string &layoutID, const FormattedText &msg,
                           const int &width, const int &height, const int &x,
                           const int &y, const std::string &textboxID);
  void const renderImage(const std::string &layoutID, const std::string &url,
                         const int &width, const int &height, const int &x,
                         const int &y, const std::string &imageID);

 public:
  WebLayout() : id(generateID()) {}
  ~WebLayout() = default;
  void addImage(const ImageLayout &image);
  void removeImage(const ImageLayout &image);
  void addTextBox(const TextBoxLayout &textBox);
  void removeTextBox(const TextBoxLayout &textBox);
  const std::vector<ImageLayout> &getImages();
  const std::vector<TextBoxLayout> &getTextBoxes();
  void toggleTextBox(const TextBoxLayout &textBox);
  void toggleImage(const ImageLayout &image);
  ImageLayout getImageFromID(const std::string& id) const;
  TextBoxLayout getTextboxFromID(const std::string& id) const;
  static std::string generateID();
  const std::string getID() { return id; }
  void activateLayout();
  void deactivateLayout();
  void setPosition(std::string id, int x, int y);
  void setSize(std::string id, int width, int height);
  bool contains(std::string id) const;
};
}  // namespace cse
