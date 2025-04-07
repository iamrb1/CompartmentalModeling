/**
 * @file WebLayoutManager.cpp
 * @author Mary Holt, Grace Fitzgerald
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

    // Check if item is being dragged
    document.addEventListener(
        "mousedown", function(e) {
          // Check if clicked element is draggable
          var draggable =
              Module.ccall("call_isMoveableObject",  // C++ function name
                           "boolean",                // return type
                           ["string"],               // argument types
                           [e.target.id]             // arguments
              );

          if (draggable) {
            makeDraggable(e.target);
          }
        });

    // Function to make an element draggable
    function makeDraggable(element) {
      var offsetX = 0;
      var offsetY = 0;

      // ChatGPT used for new position calculations
      document.onmousemove = function(e) {
        var newX = (e.clientX / window.innerWidth) * 100 - offsetX;
        var newY = (e.clientY / window.innerHeight) * 100 - offsetY;

        // Call C++ function to update position within the layout
        Module.ccall("call_updatePosition", null,
                     [ "string", "number", "number" ],
                     [ element.id.trim(), newX, newY ]);
        // Update element's visual position
        element.style.left = newX + "vw";
        element.style.top = newY + "vh";
      };

      document.onmouseup = function(e) {
        // Remove event listeners
        document.onmousemove = null;
        document.onmouseup = null;
      };
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

  if (g_manager->getLayouts().empty() ||
      g_manager->getCurrentPos() >= g_manager->getLayouts().size()) {
    std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
    return;
  }

  auto& layout = g_manager->getLayouts().at(g_manager->getCurrentPos());
  auto& textBoxes = layout->getTextBoxes();

  for (auto& tb : textBoxes) {
    if (tb.textBox->getID() == textboxId) {
      tb.textBox->getFormattedText().setText(newText);
      break;
    }
  }
}

EMSCRIPTEN_KEEPALIVE void call_updatePosition(const char* id, int newX,
                                              int newY) {
  std::string cppId(id);
  if (g_manager) {
    g_manager->updatePosition(cppId, newX, newY);
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
  }
}

EMSCRIPTEN_KEEPALIVE bool call_isMoveableObject(const char* id) {
  std::string cppId(id);
  if (g_manager) {
    return g_manager->isMoveableObject(cppId);
  } else {
    std::cout << "ERROR: g_manager is null!" << std::endl;
    return false;
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

  if (layouts.empty() || currentPos >= layouts.size()) {
    std::cout << "ERROR: No current layout to modify.\n";
    return;
  }

  layouts.at(currentPos)->addTextBox(newLayout);
}

void WebLayoutManager::addImage(const std::string& url, int width, int height,
                                const std::string& altText) {
  if (width <= 0 || height <= 0) {
    std::cout << "ERROR: Invalid image dimensions: width=" << width
              << ", height=" << height << std::endl;
    return;
  }

  if (layouts.empty() || currentPos >= layouts.size()) {
    std::cout << "ERROR: No current layout to add image.\n";
    return;
  }

  auto image = std::make_shared<Image>(url, width, height, altText);
  ImageLayout layout{image, 10, 10};
  layouts.at(currentPos)->addImage(layout);
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

/**
 * Updates object position
 */
void WebLayoutManager::updatePosition(std::string id, int newX, int newY) {
  if (layouts.empty() || currentPos >= layouts.size()) return;

  layouts.at(currentPos)->setPosition(id, newX, newY);
}

/**
 * Returns if id is a moveable item
 */
bool WebLayoutManager::isMoveableObject(std::string id) const {
  if (layouts.empty() || currentPos >= layouts.size()) return false;

  return layouts.at(currentPos)->contains(id);
}

}  // namespace cse