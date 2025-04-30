/**
 * @file MockWebLayout.cpp
 * @author Mary Holt,Grace Fitzgerald, Owen Haiar
 *
 */

#include "MockWebLayout.hpp"

#include <cassert>
#include <iostream>
#include <algorithm>

namespace cse {

// Magic Numbers
constexpr int MAX_WIDTH_PERCENT = 100;
constexpr int MAX_HEIGHT_PERCENT = 95;
constexpr int MIN_PERCENT = 0;

/**
 * Adds an image to the web layout
 * @param image to be added
 */
void MockWebLayout::addImage(const ImageLayout &image) {
  images.push_back(image);
  if ((image.image->getHeight() > MIN_PERCENT &&
       image.image->getWidth() > MIN_PERCENT) &&
      (image.xPos >= MIN_PERCENT && image.yPos >= MIN_PERCENT)) {
    renderImage(getID(), image.image->getURL(), image.image->getWidth(),
                image.image->getHeight(), image.xPos, image.yPos,
                image.image->getID());
  }
}

/**
 * Removes a specific image from the web layout
 * @param image to be removed
 */
void MockWebLayout::removeImage(const ImageLayout &image) {
  auto it = std::find(images.begin(), images.end(), image);
  if (it != images.end()) {
    images.erase(it);
  }
}

/**
 * Adds a textbox to the web layout
 * @param textBox to be added
 */
void MockWebLayout::addTextBox(const TextBoxLayout &textBox) {
  textBoxes.push_back(textBox);

  std::cout << "Adding textbox" << std::endl;

  if ((textBox.textBox->getHeight() > MIN_PERCENT &&
       textBox.textBox->getWidth() > MIN_PERCENT) &&
      (textBox.xPos >= MIN_PERCENT && textBox.yPos >= MIN_PERCENT)) {
    renderTextBox(getID(), textBox.textBox->getFormattedText(),
                  textBox.textBox->getWidth(), textBox.textBox->getHeight(),
                  textBox.xPos, textBox.yPos, textBox.textBox->getID());
  }
}

/**
 * Removes a textbox from the web layout
 * @param textBox to be removed
 */
void MockWebLayout::removeTextBox(const TextBoxLayout &textBox) {
  auto it = std::find(textBoxes.begin(), textBoxes.end(), textBox);
  if (it != textBoxes.end()) {
    textBoxes.erase(it);
  }

  // TODO: remove from layout
}

/**
 * Gets images vector of web layout
 * @return images that are connected
 */
const std::vector<ImageLayout> &MockWebLayout::getImages() { return images; }

/**
 * Gets text boxes vector of web layout
 * @return textBoxes that are connected
 */
const std::vector<TextBoxLayout> &MockWebLayout::getTextBoxes() {
  return textBoxes;
}

/**
 * Converts textbox attributes into html
 * @param msg text of the textbox
 * @param width width of the textbox
 * @param height height of the textbox
 */
const void MockWebLayout::renderTextBox(const std::string &layoutID,
                                    const FormattedText &formattedText,
                                    const int &width, const int &height,
                                    const int &x, const int &y,
                                    const std::string &textboxID) {
  assert(width >= MIN_PERCENT &&
         height >= MIN_PERCENT);  // assert width and height are positive
  assert(x >= MIN_PERCENT && y >= MIN_PERCENT);  // assert x and y are positive

  // Get Formatting variables
  auto text = formattedText.getText();
  auto color = formattedText.getColor();
  auto font = formattedText.getFont();
  auto fontSize = formattedText.getFontSize();

}

/**
 * Converts image attributes into html
 * @param url of image source
 * @param width of image
 * @param height of images
 * @param xLoc of image
 * @param yLoc of image
 */
void const MockWebLayout::renderImage(const std::string &layoutID,
                                  const std::string &url, const int &width,
                                  const int &height, const int &x, const int &y,
                                  const std::string &imageID) {
  assert(width >= MIN_PERCENT &&
         height >= MIN_PERCENT);  // assert width and height are positive
  assert(x >= MIN_PERCENT && y >= MIN_PERCENT);  // assert x and y are positive

}

/**
 * Generates ID of layout
 * @return unique ID
 */
std::string MockWebLayout::generateID() {
  static int counter = 0;
  return "layout-" + std::to_string(counter++);
}

/**
 * Activates layout on html
 */
void MockWebLayout::activateLayout() {
  auto layoutID = getID();

  //std::cout << "Activating:  " << layoutID << std::endl;

}

/**
 * Deactivates layout on html
 */
void MockWebLayout::deactivateLayout() {
  auto layoutID = getID();

  //std::cout << "Deactivating:  " << layoutID << std::endl;

}


void MockWebLayout::toggleImage(const cse::ImageLayout &image) {
  auto imageID = image.image->getID();


}
ImageLayout MockWebLayout::getImageFromID(const std::string& id) const {
  if (const auto image = std::ranges::find_if(images,[id](const ImageLayout& im) { return im.image->getID() == id; }); image != images.end()) {
    return *image;
  }
  throw std::runtime_error("Image not found");
}

TextBoxLayout MockWebLayout::getTextboxFromID(const std::string& id) const {
  if (const auto textbox = std::ranges::find_if(textBoxes,[id](const TextBoxLayout& tb) { return tb.textBox->getID() == id; }); textbox != textBoxes.end()) {
    return *textbox;
  }
  throw std::runtime_error("Textbox not found");
}


void MockWebLayout::toggleTextBox(const cse::TextBoxLayout &textBox) {
  auto textBoxID = textBox.textBox->getID();

}


/**
 * Converts image attributes into html
 * @param id : Object id
 * @param newX : New x value location for object to be set at
 * @param newY : New y value location for object to be set at
 */
void MockWebLayout::setPosition(std::string id, int newX, int newY) {
  for (auto& tbl : textBoxes) {
    if (tbl.textBox->getID() == id) {
      tbl.setPosition(newX, newY);
    }
  }

  for (auto& imgl : images) {
    if (imgl.image->getID() == id) {
      imgl.setPosition(newX, newY);
    }
  }
}

/**
 * Converts image attributes into html
 * @param id : Object id
 * @param newWidth : New width for object
 * @param newHeight : New height for object
 */
void MockWebLayout::setSize(std::string id, int newWidth, int newHeight) {
  for (auto& tbl : textBoxes) {
    if (tbl.textBox->getID() == id) {
      tbl.textBox->resize(newWidth, newHeight);
    }
  }

  for (auto& imgl : images) {
    if (imgl.image->getID() == id) {
      imgl.image->resize(newWidth, newHeight);
    }
  }
}

/**
 * Checks if MockWebLayout contains object with matching id
 * @param id of object
 * @return Boolean indicating if weblayout contains object
 */
bool MockWebLayout::contains(std::string id) const {
  for (const auto& tbl : textBoxes) {
    if (tbl.textBox->getID() == id) {
      return true;
    }
  }

  for (const auto& imgl : images) {
    if (imgl.image->getID() == id) {
      return true;
    }
  }

  return false;
}


}  // namespace cse