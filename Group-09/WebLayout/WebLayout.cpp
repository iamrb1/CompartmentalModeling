/**
 * @file WebLayout.cpp
 * @author Mary Holt
 *
 */

#include "WebLayout.hpp"

#include <cassert>
#include <iostream>

namespace cse {

// Magic Numbers
constexpr int MAX_WIDTH_PERCENT = 100;
constexpr int MAX_HEIGHT_PERCENT = 95;
constexpr int MIN_PERCENT = 0;

/**
 * Adds an image to the web layout
 * @param image to be added
 */
void WebLayout::addImage(const ImageLayout &image) {
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
void WebLayout::removeImage(const ImageLayout &image) {
  auto it = std::find(images.begin(), images.end(), image);
  if (it != images.end()) {
    images.erase(it);
  }
}

/**
 * Adds a textbox to the web layout
 * @param textBox to be added
 */
void WebLayout::addTextBox(const TextBoxLayout &textBox) {
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
void WebLayout::removeTextBox(const TextBoxLayout &textBox) {
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
const std::vector<ImageLayout> &WebLayout::getImages() { return images; }

/**
 * Gets text boxes vector of web layout
 * @return textBoxes that are connected
 */
const std::vector<TextBoxLayout> &WebLayout::getTextBoxes() {
  return textBoxes;
}

/**
 * Converts textbox attributes into html
 * @param msg text of the textbox
 * @param width width of the textbox
 * @param height height of the textbox
 */
const void WebLayout::renderTextBox(const std::string &layoutID,
                                    const FormattedText &formattedText, const int &width,
                                    const int &height, const int &x,
                                    const int &y,
                                    const std::string &textboxID) {
  assert(width >= MIN_PERCENT &&
      height >= MIN_PERCENT);  // assert width and height are positive
  assert(x >= MIN_PERCENT && y >= MIN_PERCENT);  // assert x and y are positive

  // Get Formatting variables
  auto text = formattedText.getText();
  auto color = formattedText.getColor();
  auto font = formattedText.getFont();
  auto fontSize = formattedText.getFontSize();

  EM_ASM(
      {
        var layoutID = UTF8ToString($0);
        var msg = UTF8ToString($1);
        var width = $2;
        var height = $3;
        var x = $4;
        var y = $5;
        var textboxID = UTF8ToString($6);
        var MAX_WIDTH_PERCENT = $7;
        var MAX_HEIGHT_PERCENT = $8;
        var color = UTF8ToString($9);
        var font = UTF8ToString($10);
        var fontSize = $11;


        // Calculate the ratio to view height/width (1%)
        var widthRatio =
            document.documentElement.clientWidth / MAX_WIDTH_PERCENT;
        var heightRatio =
            document.documentElement.clientHeight / MAX_WIDTH_PERCENT;

        // Don't let placement exceed 100% of view width/height
        if (width > MAX_WIDTH_PERCENT) {
          width = MAX_WIDTH_PERCENT;
        }
        if (height > MAX_HEIGHT_PERCENT) {
          height = MAX_HEIGHT_PERCENT;
        }
        if (x > MAX_WIDTH_PERCENT) {
          x = MAX_WIDTH_PERCENT - width;
        }
        if (y > MAX_HEIGHT_PERCENT) {
          y = MAX_HEIGHT_PERCENT - height;
        }

        // Finds div with presentation-zone id, where all boxes and images will
        // be placed
        var textBoxDiv = document.getElementById("presentation-zone");

        var layoutDiv = document.getElementById(layoutID);
        if (!layoutDiv) {
          // If it doesn't exist, create div
          layoutDiv = document.createElement("div");
          layoutDiv.style.visibility = "hidden";
          layoutDiv.id = layoutID;
          textBoxDiv.appendChild(layoutDiv);
        }
        console.log(color);

        // position: absolute; left: x px; top: y px;

        if (layoutDiv) {
          // Check if textbox already exists in layout
          var currentTextBox = document.getElementById(textboxID);
          if (!currentTextBox) {
            layoutDiv.innerHTML +=
                '<p id=' + textboxID + ' style="position: absolute; left: ' +
                    x + 'vw; top: ' + y +
                    'vh; border: 1px solid black; margin: 0; border-radius: 5px; height: ' +
                    height + 'vh; width: ' + width + 'vw; color: ' + color + '; font-family: ' + font + '; font-size:'
                    + fontSize + 'px;">' + msg + '</p>';
          }
        }
      },
      layoutID.c_str(), text.c_str(), width, height, x, y, textboxID.c_str(),
      MAX_WIDTH_PERCENT, MAX_HEIGHT_PERCENT, color.c_str(), font.c_str(), fontSize);
}

/**
 * Converts image attributes into html
 * @param url of image source
 * @param width of image
 * @param height of images
 * @param xLoc of image
 * @param yLoc of image
 */
void const WebLayout::renderImage(const std::string &layoutID,
                                  const std::string &url, const int &width,
                                  const int &height, const int &x, const int &y,
                                  const std::string &imageID) {
  assert(width >= MIN_PERCENT &&
      height >= MIN_PERCENT);  // assert width and height are positive
  assert(x >= MIN_PERCENT && y >= MIN_PERCENT);  // assert x and y are positive

  EM_ASM(
      {
        var layoutID = UTF8ToString($0);
        var msg = UTF8ToString($1);
        var width = $2;
        var height = $3;
        var x = $4;
        var y = $5;
        var imageID = UTF8ToString($6);
        var MAX_WIDTH_PERCENT = $7;
        var MAX_HEIGHT_PERCENT = $8;

        // Don't let placement exceed 100% of view width/height or 95% height
        if (width > MAX_WIDTH_PERCENT) {
          width = MAX_WIDTH_PERCENT;
        }
        if (height > MAX_HEIGHT_PERCENT) {
          height = MAX_HEIGHT_PERCENT;
        }
        if (x > MAX_WIDTH_PERCENT) {
          x = MAX_WIDTH_PERCENT - width;
        }
        if (y > MAX_HEIGHT_PERCENT) {
          y = MAX_HEIGHT_PERCENT - height;
        }

        // Finds div with presentation-zone id, where all boxes and images will
        // be placed
        var imageBoxDiv = document.getElementById("presentation-zone");

        var layoutDiv = document.getElementById(layoutID);
        if (!layoutDiv) {
          // If it doesn't exist, create div
          layoutDiv = document.createElement("div");
          layoutDiv.style.visibility = "hidden";
          layoutDiv.id = layoutID;
          imageBoxDiv.appendChild(layoutDiv);
        }

        if (layoutDiv) {
          layoutDiv.innerHTML +=
              "<img id=" + imageID + " src='" + msg +
                  "' style='position: absolute; left: " + x + "vw; top: " + y +
                  "vh; margin: 0; object-fit: contain; width:" + width +
                  "vw; height:" + height + "vh;' />";
        }
      },
      layoutID.c_str(), url.c_str(), width, height, x, y, imageID.c_str(),
      MAX_WIDTH_PERCENT, MAX_HEIGHT_PERCENT);
}

/**
 * Loads all text boxes and images on current web layout
 */
void WebLayout::loadPage() {
  // Display text boxes
  for (const auto &layout : textBoxes) {
    // Future add functionality to fully format text

    // Verify values are valid
    if ((layout.textBox->getHeight() > MIN_PERCENT &&
        layout.textBox->getWidth() > MIN_PERCENT) &&
        (layout.xPos >= MIN_PERCENT && layout.yPos >= MIN_PERCENT)) {
      renderTextBox(getID(), layout.textBox->getFormattedText(),
                    layout.textBox->getWidth(), layout.textBox->getHeight(),
                    layout.xPos, layout.yPos, layout.textBox->getID());
    }
  }

  // Display images
  for (const auto &layout : images) {
    // Verify values are valid
    if ((layout.image->getHeight() > MIN_PERCENT &&
        layout.image->getWidth() > MIN_PERCENT) &&
        (layout.xPos >= MIN_PERCENT && layout.yPos >= MIN_PERCENT)) {
      renderImage(getID(), layout.image->getURL(), layout.image->getWidth(),
                  layout.image->getHeight(), layout.xPos, layout.yPos,
                  layout.image->getID());
    }
  }
}

/**
 * Generates ID of layout
 * @return unique ID
 */
std::string WebLayout::generateID() {
  static int counter = 0;
  return "layout-" + std::to_string(counter++);
}

/**
 * Activates layout on html
 */
void WebLayout::activateLayout() {
  auto layoutID = getID();

  std::cout << "Activating:  " << layoutID << std::endl;

  EM_ASM(
      {
        var layoutID = UTF8ToString($0);

        var layoutDiv = document.getElementById(layoutID);
        if (layoutDiv) {
          layoutDiv.style.visibility = "visible";
        }
      },
      layoutID.c_str());
}

/**
 * Deactivates layout on html
 */
void WebLayout::deactivateLayout() {
  auto layoutID = getID();

  std::cout << "Deactivating:  " << layoutID << std::endl;

  EM_ASM(
      {
        var layoutID = UTF8ToString($0);
        var layoutDiv = document.getElementById(layoutID);
        if (layoutDiv) {
          layoutDiv.style.visibility = "hidden";
        }
      },
      layoutID.c_str());
}


/**
 * Converts image attributes into html
 * @param id : Object id
 * @param newX : New x value location for object to be set at
 * @param newY : New y value location for object to be set at
 */
void WebLayout::setPosition(std::string id, int newX, int newY) {
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
 * Checks if WebLayout contains object with matching id
 * @param id of object
 * @return Boolean indicating if weblayout contains object
 */
bool WebLayout::contains(std::string id) const {
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