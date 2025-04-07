/**
 * @file WebLayoutManager.cpp
 * @author Mary Holt
 */

#include "WebLayoutManager.hpp"

#include <emscripten/bind.h>

namespace cse {

/**
 * Global pointer for use in JS
 */
WebLayoutManager* g_manager = nullptr;

/**
 * Constructs manager and binds buttons on html to functions
 */
WebLayoutManager::WebLayoutManager() {
  g_manager = this;

  // Set up the event listener for the button click
  EM_ASM({
    // Setup advance button
    var advanceButton = document.getElementById("advanceButton");
    if (advanceButton) {
      advanceButton.addEventListener(
          "click", function() { Module._call_advance(); });
    }

    var rewindButton = document.getElementById("reverseButton");
    if (rewindButton) {
      rewindButton.addEventListener(
          "click", function() { Module._call_rewind(); });
    }

    var addTextBoxButton = document.getElementById("addTextBoxButton");
    if (addTextBoxButton) {
      addTextBoxButton.addEventListener(
          "click", function() { Module._call_addTextBox(); });
    }

    var addImageButton = document.getElementById("addImageButton");
    if (addImageButton) {
      addImageButton.addEventListener(
          "click", function() { Module._call_addImage(); });
    }

    var addNewSlide = document.getElementById("addNewSlideButton");
    if (addNewSlide) {
      addNewSlide.addEventListener(
          "click", function() { Module._call_addNewSlide(); });
    }
  });
}

extern "C" {
EMSCRIPTEN_KEEPALIVE void call_advance() {
  if (g_manager) {
    g_manager->advance();
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
  }
}

EMSCRIPTEN_KEEPALIVE void call_rewind() {
  if (g_manager) {
    g_manager->rewind();
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
  }
}

EMSCRIPTEN_KEEPALIVE void call_addImage(char* urlPtr, int width, int height,
                                        char* altPtr) {
  std::string url(urlPtr);
  std::string altText(altPtr);

  if (g_manager) {
    g_manager->addImage(url, width, height, altText);
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
  }
}

EMSCRIPTEN_KEEPALIVE void call_addTextBox() {
  if (g_manager) {
    g_manager->addTextBox();
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
  }
}

EMSCRIPTEN_KEEPALIVE void call_addNewSlide() {
  if (g_manager) {
    g_manager->addNewSlide();
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
  }
}

EMSCRIPTEN_KEEPALIVE
void updateTextBoxContent(const char* textboxId, const char* newText) {
  if (!g_manager) return;

  auto& layout = g_manager->getLayouts().at(g_manager->getCurrentPos());
  auto& textBoxes = layout->getTextBoxes();

  for (auto& tb : textBoxes) {
    if (tb.textBox->getID() == textboxId) {
      tb.textBox->getFormattedText().setText(newText);
      break;
    }
  }
}
}

/**
 * Adds a layout to this manager
 * @param layout
 */
void WebLayoutManager::addLayout(std::shared_ptr<WebLayout> layout) {
  layouts.push_back(layout);
}

/**
 * Removes a layout to this manager
 * @param layout
 */
void WebLayoutManager::removeLayout(std::shared_ptr<WebLayout> layout) {
  layouts.erase(std::remove(layouts.begin(), layouts.end(), layout),
                layouts.end());
}

/**
 * Advances to the next layout
 */
void WebLayoutManager::advance() {
  EM_ASM({ console.log("Advancing"); });

  if (layouts.empty()) {
    EM_ASM({ console.error("ERROR: No layouts exist."); });
    return;  // Exit early
  }

  if (currentPos >= layouts.size() - 1) {
    EM_ASM({ console.error("ERROR: Currently at last layout."); });
    return;  // Exit early
  }

  auto currentLayout = layouts.at(currentPos);
  if (currentLayout) {
    currentLayout->deactivateLayout();
  }

  currentPos++;

  currentLayout = layouts.at(currentPos);
  if (currentLayout) {
    currentLayout->activateLayout();
  } else {
    currentPos--;
  }
}

/**
 * Rewinds to the previous layout
 */
void WebLayoutManager::rewind() {
  EM_ASM({ console.log("Rewinding"); });

  if (layouts.empty()) {
    EM_ASM({ console.error("ERROR: No layouts exist."); });
    return;  // Exit early
  }

  if (currentPos <= 0) {
    EM_ASM({ console.error("ERROR: Currently at first layout."); });
    return;  // Exit early
  }

  auto currentLayout = layouts.at(currentPos);
  if (currentLayout) {
    currentLayout->deactivateLayout();
  }

  currentPos--;

  currentLayout = layouts.at(currentPos);
  if (currentPos >= 0 && currentPos < layouts.size()) {
    currentLayout->activateLayout();
  } else {
    currentPos++;
  }
}

/**
 * Initializes the visual to the first slide if it exists
 */
void WebLayoutManager::initialize() {
  auto currentLayout = layouts.at(0);
  if (currentLayout) {
    currentLayout->activateLayout();
  }
}

void WebLayoutManager::addTextBox() {
  // Create a new text box with predefined properties
  FormattedText ft;
  ft.setText("New Box!");
  TextBoxConfig tbc;
  tbc.content = ft;
  tbc.height = 10;
  tbc.width = 30;
  auto newTextBox = std::make_shared<TextBox>("", tbc);
  TextBoxLayout newLayout = {newTextBox, 50, 50};

  layouts.at(currentPos)->addTextBox(newLayout);
  // layouts.at(currentPos)->loadPage();
}

void WebLayoutManager::addImage(const std::string& url, int width, int height,
                                const std::string& altText) {
  auto image = std::make_shared<Image>(url, width, height, altText);
  ImageLayout layout{image, 10, 10};  // Default x/y position
  layouts.at(currentPos)->addImage(layout);
}

/**
 * Go to a given slide
 * @param slide_num
 */
void WebLayoutManager::goTo(const size_t slide_num) {
  EM_ASM({console.log("Changing slide"); });

  if (slide_num < 0 || slide_num >= layouts.size()) {
    EM_ASM({console.error("ERROR: Requested slide is OOB."); });
    return; // Exit early
  }

  auto currentLayout = layouts.at(currentPos);
  if (currentLayout) {
    currentLayout->deactivateLayout();
  }

  currentPos = slide_num;

  currentLayout = layouts.at(currentPos);
  currentLayout->activateLayout();
}

void WebLayoutManager::addNewSlide() {
  // Create a new weblayout
  auto wb = std::make_shared<WebLayout>();
  addLayout(wb);
  auto wbID = wb->getID();
  EM_ASM(
      {
        var wbID = UTF8ToString($0);
        console.log("Added new slide: ", wbID);
      },
      wbID.c_str());
}

}  // namespace cse