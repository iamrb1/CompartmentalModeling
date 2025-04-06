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
                 "click", function()
             { Module._call_advance(); });
           }

           var rewindButton = document.getElementById("reverseButton");
           if (rewindButton) {
             rewindButton.addEventListener(
                 "click", function()
             { Module._call_rewind(); });
           }

           var addTextBoxButton = document.getElementById("addTextBoxButton");
           if (addTextBoxButton) {
             addTextBoxButton.addEventListener(
                 "click", function()
             { Module._call_addTextBox(); });
           }

           var addNewSlide = document.getElementById("addNewSlideButton");
           if (addNewSlide) {
             addNewSlide.addEventListener(
                 "click", function()
             { Module._call_addNewSlide(); });
           }

  // Check if item is being dragged

    document.addEventListener('mousedown', function(e) {
//      var clickedElement = e.target;
	  console.log("SCREEN CLICKED");
      console.log("Passed ID to C++:", e.target.id);
      console.log("CLICKED ELEMENT", e.target);
      // Check if clicked element is draggable (e.g., a textbox or image)
      var draggable = Module.ccall(
  'call_isMoveableObject', // C++ function name
  'boolean',                 // return type (1 = true, 0 = false)
  ['string'],               // argument types
  [e.target.id]              // arguments
);
      console.log("DRAGGABLE:",draggable, "ID", e.target.id);
if ( draggable ) {

  console.log("CLICKED ELEMENT", e.target);
//  clickedElement.setAttribute("data-draggable", "true"); // prevent re-binding
  makeDraggable(e.target);
}

    });

//    // Function to make an element draggable
    function makeDraggable(element) {
        var offsetX = 0;
        var offsetY = 0;
		console.log("MAKE DRAGGABLE", element);


//      element.onmousedown = function(e) {
//        console.log("onmousedown");
//        e.preventDefault(); // Prevent text selection
//	    offsetX = (e.clientX / window.innerWidth) * 100 - (element.getBoundingClientRect().left / window.innerWidth) * 100;
//    	offsetY = (e.clientY / window.innerHeight) * 100 - (element.getBoundingClientRect().top / window.innerHeight) * 100;


//        // Set up mousemove and mouseup handlers
        document.onmousemove = function(e) {
      var newX = (e.clientX / window.innerWidth) * 100 - offsetX;
      var newY = (e.clientY / window.innerHeight) * 100 - offsetY;
console.log("onmousemove");
          // Update element position

			//instead of changing style call c++ function
		//Used ChatGPT to figure out conversions between JS and C++ types
		Module.ccall('call_updatePosition', null, ['string', 'number', 'number'], [element.id.trim(), newX, newY]);
          console.log("Position updated to:", newX, newY);
          element.style.left = newX + 'vw';
          element.style.top = newY + 'vh';
        };
//
        document.onmouseup = function(e) {
      // Calculate final position and move the element
      console.log("ONMOUSEUP");
      // Remove event listeners after mouse is released
      document.onmousemove = null;
      document.onmouseup = null;
        };
//      };
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

EMSCRIPTEN_KEEPALIVE void call_updatePosition(std::string id, int newX, int newY) {
  if (g_manager) {
    g_manager->updatePosition(id, newX, newY);
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

  layouts.at(currentPos)->addTextBox(newLayout);
  //layouts.at(currentPos)->loadPage();
}

void WebLayoutManager::addNewSlide() {
  // Create a new weblayout
  auto wb = std::make_shared<WebLayout>();
  addLayout(wb);
  auto wbID = wb->getID();
  EM_ASM({
           var wbID = UTF8ToString($0);
           console.log("Added new slide: ", wbID);
         }, wbID.c_str());
}

void WebLayoutManager::updatePosition(std::string id, int newX, int newY) {
  // Updates items position
    layouts.at(currentPos)->setPosition(id, newX, newY);
}

bool WebLayoutManager::isMoveableObject(std::string id) const{
	return layouts.at(currentPos)->contains(id);
}

}  // namespace cse