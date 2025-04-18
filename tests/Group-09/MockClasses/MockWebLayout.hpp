/**
 * Holds various text boxes and images to create "slides"
 *
 * @file MockWebLayout.hpp
 * @author Mary Holt, Grace Fitzgerald
 *
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "MockImage.hpp"
#include "../../../Group-09/TextBox/TextBox.hpp"


struct TextBoxLayout {
  std::shared_ptr<cse::TextBox> textBox;
  int yPos;
  int xPos;

  TextBoxLayout() : textBox(), yPos(0), xPos(0){};

  TextBoxLayout(std::shared_ptr<cse::TextBox> textBox, int x, int y)
      : textBox(std::move(textBox)), yPos(x), xPos(y) {}

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
  std::shared_ptr<MockImage> image;
  int yPos;
  int xPos;

  ImageLayout() : image(), yPos(0), xPos(0){};

  ImageLayout(std::shared_ptr<MockImage> image, int x, int y)
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

class MockWebLayout {
 private:
  std::vector<TextBoxLayout> textBoxes;
  std::vector<ImageLayout> images;
  std::string id;

  void renderTextBox(const std::string &layoutID, const cse::FormattedText &msg,
                           const int &width, const int &height, const int &x,
                           const int &y, const std::string &textboxID) const;
  void renderImage(const std::string &layoutID, const std::string &url,
                         const int &width, const int &height, const int &x,
                         const int &y, const std::string &imageID) const;

 public:
  MockWebLayout() : id(generateID()) {}
  ~MockWebLayout() = default;
  void addImage(const ImageLayout &image);
  void removeImage(const ImageLayout &image);
  void addTextBox(const TextBoxLayout &textBox);
  void removeTextBox(const TextBoxLayout &textBox);
  const std::vector<ImageLayout> &getImages();
  const std::vector<TextBoxLayout> &getTextBoxes();
  void toggleTextBox(const TextBoxLayout &textBox);
  void toggleImage(const ImageLayout &image);

  static std::string generateID();
  const std::string getID() { return id; }

  // TODO: Future Implement Clear Layout Function
  void activateLayout();
  void deactivateLayout();

  void setPosition(std::string id, int x, int y);
  bool contains(std::string id) const;
};

