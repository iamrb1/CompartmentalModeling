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
WebLayoutManager *g_manager = nullptr;

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
  });

  EM_ASM({
    // Setup advance button
    var rewindButton = document.getElementById("reverseButton");
    if (rewindButton) {
      rewindButton.addEventListener(
          "click", function() { Module._call_rewind(); });
    }
  });
}

extern "C" {
EMSCRIPTEN_KEEPALIVE void call_advance() {
  if (g_manager) {
    g_manager->advance();
  } else {
    std::cerr << "Error: g_manager is null!" << std::endl;
  }
}
}
extern "C" {

EMSCRIPTEN_KEEPALIVE void call_rewind() {
  if (g_manager) {
    g_manager->rewind();
  } else {
    std::cerr << "Error: g_manager is null!" << std::endl;
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
}  // namespace cse