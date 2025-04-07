/**
 * @file PresentationManager.h
 * @brief This file contains the PresentationManager and the exported functions called by JS
 * @author Owen Haiar, Mary Holt, Allie Ianello
 */

#pragma once

#include "PresentationEventManager.hpp"
#include "../WebLayout/WebLayout.hpp"
#include <nlohmann/json.hpp>

using namespace cse;
using json = nlohmann::json;
// Aliases from PresentationEventManager:
//	using LayoutManager = std::shared_ptr<WebLayoutManager>;
//	using PresentationFunction = std::function<void(LayoutManager, size_t)>;

/**
 * Bind the JS to the C functions
 */
void bind() {
	// Set up the event listener for the button click
	EM_ASM({
		// Setup advance button
    var advanceButton = document.getElementById("advanceButton");
    if (advanceButton) {
			advanceButton.addEventListener(
				"click",
				function() {Module._call_advance(); });


		}

    var rewindButton = document.getElementById("reverseButton");
    if (rewindButton) {
			rewindButton.addEventListener(
				"click",
				function() {Module._call_rewind(); });


		}

    var addTextBoxButton = document.getElementById("addTextBoxButton");
    if (addTextBoxButton) {
			addTextBoxButton.addEventListener(
				"click",
				function() {Module._call_addTextBox(); });


		}

    var addImageButton = document.getElementById("addImageButton");
    if (addImageButton) {
			addImageButton.addEventListener(
				"click",
				function() {Module._call_addImage(); });


		}

    var addNewSlide = document.getElementById("addNewSlideButton");
    if (addNewSlide) {
			addNewSlide.addEventListener(
				"click",
				function() {Module._call_addNewSlide(); });


		}
	var startButton = document.getElementById("startButton");
	if (startButton) {
			startButton.addEventListener(
				"click",
				function() {Module._call_start(); });


		}
	var stopButton = document.getElementById("stopButton");
	if (stopButton) {
			stopButton.addEventListener(
				"click",
				function() {Module._call_stop(); });


		}
	var addTransition = document.getElementById("addTransition");
	if (addTransition) {
			addTransition.addEventListener(
				"click",
				function() {Module._call_addSlideChangeEvent(); });


		}
		// Check if item is being dragged
		document.addEventListener(
			"mousedown",
			function(e) {
			// Check if clicked element is draggable
          var draggable =
			Module.ccall("call_isMoveableObject",
			             // C++ function name
			             "boolean",
			             // return type
			             ["string"],
			             // argument types
			             [e.target.id] // arguments
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
				Module.ccall("call_updatePosition",
				             null,
				             [ "string",
				             "number",
				             "number"],
				             [ element.id.trim(),
				             newX,
				             newY]);
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

/**
 * Main manager for Liskov presentation program
 * Initialized globally, and update() is called in a loop while running
 */
class PresentationManager {
	private:
		PresentationEventManager _event_manager;
		bool _running = false;
		int _current_pos = 0;
		std::vector<std::shared_ptr<WebLayout> > _layouts;

	public:
		PresentationManager() : _event_manager(this) {
			bind();
		}

		/**
         * Adds a layout to this manager
         * @param layout
         */
		void addLayout(std::shared_ptr<WebLayout> layout) {
			_layouts.push_back(layout);
			_event_manager.resize(_layouts.size());
		}

		/**
		 * Removes a layout to this manager
		 * @param layout
		 */
		void removeLayout(std::shared_ptr<WebLayout> layout) {
			_layouts.erase(std::remove(_layouts.begin(), _layouts.end(), layout),
			              _layouts.end());
		}

		/**
		 * Advances to the next layout
		 */
		void advance() {
			EM_ASM({console.log("Advancing"); });

			if (_layouts.empty()) {
				EM_ASM({console.error("ERROR: No layouts exist."); });
				return; // Exit early
			}

			if (_current_pos >= _layouts.size() - 1) {
				EM_ASM({console.error("ERROR: Currently at last layout."); });
				return; // Exit early
			}

			auto currentLayout = _layouts.at(_current_pos);
			if (currentLayout) {
				currentLayout->deactivateLayout();
			}

			_current_pos++;

			currentLayout = _layouts.at(_current_pos);
			if (currentLayout) {
				currentLayout->activateLayout();
			} else {
				_current_pos--;
			}
			_event_manager.onSlideChanged(_current_pos);
		}

		/**
		 * Rewinds to the previous layout
		 */
		void rewind() {
			EM_ASM({console.log("Rewinding"); });

			if (_layouts.empty()) {
				EM_ASM({console.error("ERROR: No layouts exist."); });
				return; // Exit early
			}

			if (_current_pos <= 0) {
				EM_ASM({console.error("ERROR: Currently at first layout."); });
				return; // Exit early
			}

			auto currentLayout = _layouts.at(_current_pos);
			if (currentLayout) {
				currentLayout->deactivateLayout();
			}

			_current_pos--;

			currentLayout = _layouts.at(_current_pos);
			if (_current_pos >= 0 && _current_pos < _layouts.size()) {
				currentLayout->activateLayout();
			} else {
				_current_pos++;
			}
			_event_manager.onSlideChanged(_current_pos);
		}

		/**
		 * Initializes the visual to the first slide if it exists
		 */
		void initialize() {
			auto currentLayout = _layouts.at(0);
			if (currentLayout) {
				currentLayout->activateLayout();
			}
		}

		void addTextBox() {
			// Create a new text box with predefined properties
			FormattedText ft;
			ft.setText("New Box!");
			TextBoxConfig tbc;
			tbc.content = ft;
			tbc.height = 10;
			tbc.width = 30;
			auto newTextBox = std::make_shared<TextBox>("", tbc);
			TextBoxLayout newLayout = {newTextBox, 50, 50};

			if (_layouts.empty() || _current_pos >= _layouts.size()) {
				std::cout << "ERROR: No current layout to modify.\n";
				return;
			}

			_layouts.at(_current_pos)->addTextBox(newLayout);
			_layouts.at(_current_pos)->activateLayout();
		}

		void addImage(const std::string &url,
		              int width,
		              int height,
		              const std::string &altText) {
			if (width <= 0 || height <= 0) {
				std::cout << "ERROR: Invalid image dimensions: width=" << width
						<< ", height=" << height << std::endl;
				return;
			}

			if (_layouts.empty() || _current_pos >= _layouts.size()) {
				std::cout << "ERROR: No current layout to add image.\n";
				return;
			}

			auto image = std::make_shared<Image>(url, width, height, altText);
			ImageLayout layout{image, 10, 10};
			_layouts.at(_current_pos)->addImage(layout);
			_layouts.at(_current_pos)->activateLayout();
		}

		/**
		 * Go to a given slide
		 * @param slide_num
		 */
		void goTo(const size_t slide_num) {
			EM_ASM({console.log("Changing slide"); });

			if (slide_num < 0 || slide_num >= _layouts.size()) {
				EM_ASM({console.error("ERROR: Requested slide is OOB."); });
				return; // Exit early
			}

			auto currentLayout = _layouts.at(_current_pos);
			if (currentLayout) {
				currentLayout->deactivateLayout();
			}

			_current_pos = slide_num;

			currentLayout = _layouts.at(_current_pos);
			currentLayout->activateLayout();
			_event_manager.onSlideChanged(_current_pos);
		}

		void addNewSlide() {
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
		void updatePosition(std::string id, int newX, int newY) {
			if (_layouts.empty() || _current_pos >= _layouts.size()) return;

			_layouts.at(_current_pos)->setPosition(id, newX, newY);
		}

		/**
		 * Returns if id is a moveable item
		 */
		bool isMoveableObject(std::string id) const {
			if (_layouts.empty() || _current_pos >= _layouts.size()) return false;

			return _layouts.at(_current_pos)->contains(id);
		}

		// Getter for export support
		const std::vector<std::shared_ptr<WebLayout> > &getLayouts() const {
			return _layouts;
		}

		/**
		 * @brief Get the Current Position of the manager
		 * @return int current position in the layout vector
		 */
		const int getCurrentPos() const { return _current_pos; }

		void start() {
			_event_manager.start();
			EM_ASM({console.log("Starting Events"); });
		}
		void stop() {
			_event_manager.stop();
			EM_ASM({console.log("Pausing Events"); });
		}

		void update() { _event_manager.update(); }

		void addSlideChangeEvent(const int time, const size_t origin, const size_t destination) {
			_event_manager.addEvent(changeSlide, origin, destination, time);
		}

		////////////////////////////////////////////////////////////////////////////////////////
		/// STATIC EVENT FUNCTIONS
		/// These are passed to the Events to allow Events to change the layouts
		////////////////////////////////////////////////////////////////////////////////////////
		static void changeSlide(PresentationManager *presentation_manager,
		                        size_t slideNum) {
			presentation_manager->goTo(slideNum);
		}
};

PresentationManager PRESENTATION_MANAGER; ///< Global presentation manager

////////////////////////////////////////////////////////////////////////////////////////
/// Exported Emscripten functions
////////////////////////////////////////////////////////////////////////////////////////
extern "C" {
EMSCRIPTEN_KEEPALIVE
void call_stop() { PRESENTATION_MANAGER.stop(); }
void call_start() { PRESENTATION_MANAGER.start(); }
void call_advance() { PRESENTATION_MANAGER.advance(); }
void call_rewind() { PRESENTATION_MANAGER.rewind(); }
void call_addImage(char *urlPtr, int width, int height, char *altPtr) {
	const std::string url(urlPtr);
	const std::string altText(altPtr);
	PRESENTATION_MANAGER.addImage(url, width, height, altText);
}
void call_updateTextBoxContent(const char* textboxId, const char* newText) {

	if (PRESENTATION_MANAGER.getLayouts().empty() ||
		PRESENTATION_MANAGER.getCurrentPos() >= PRESENTATION_MANAGER.getLayouts().size()) {
		std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
		return;
		}

	auto& layout = PRESENTATION_MANAGER.getLayouts().at(PRESENTATION_MANAGER.getCurrentPos());
	auto& textBoxes = layout->getTextBoxes();

	for (auto& tb : textBoxes) {
		if (tb.textBox->getID() == textboxId) {
			tb.textBox->getFormattedText().setText(newText);
			break;
		}
	}
}
void call_updatePosition(const char* id, int newX, int newY) {
	std::string cppId(id);
	PRESENTATION_MANAGER.updatePosition(cppId, newX, newY);
}
void call_addTextBox() { PRESENTATION_MANAGER.addTextBox(); }
void call_addNewSlide() { PRESENTATION_MANAGER.addNewSlide(); }
bool call_isMoveableObject(std::string id) { return PRESENTATION_MANAGER.isMoveableObject(std::move(id)); }
void loadSlideDeckFromJson(const char *jsonStr) {
	json deck = json::parse(jsonStr);

	for (const auto &slide : deck["slides"]) {
		auto layout = std::make_shared<WebLayout>();

		// Load all text boxes
		if (slide.contains("textBoxes")) {
			for (const auto &tbJson : slide["textBoxes"]) {
				FormattedTextConfig ftConfig{
					tbJson["text"], TextType::Raw,
					tbJson["font"], tbJson["fontSize"],
					tbJson["color"]
				};
				TextBoxConfig tbConfig{
					FormattedText(ftConfig), tbJson["width"],
					tbJson["height"]
				};
				auto textbox = std::make_shared<TextBox>("", tbConfig);
				layout->addTextBox(TextBoxLayout(textbox, tbJson["x"], tbJson["y"]));
			}
		}

		// Load all images
		if (slide.contains("images")) {
			for (const auto &imgJson : slide["images"]) {
				auto image = std::make_shared<Image>(imgJson["url"],
				                                     imgJson["width"],
				                                     imgJson["height"]);
				layout->addImage(ImageLayout(image, imgJson["x"], imgJson["y"]));
			}
		}
		PRESENTATION_MANAGER.addLayout(layout); // Add to the global manager
	}
	PRESENTATION_MANAGER.initialize(); // Show the first slide
}
const char *exportSlideDeckToJson() {
	static std::string exportedJson;
	json deck;
	deck["slides"] = json::array();

	for (const auto &layout : PRESENTATION_MANAGER.getLayouts()) {
		json slide;

		slide["textBoxes"] = json::array();
		for (const auto &tb : layout->getTextBoxes()) {
			const auto &text = tb.textBox->getFormattedText();
			slide["textBoxes"].push_back({
				{"text", text.getText()},
				{"font", text.getFont()},
				{"fontSize", text.getFontSize()},
				{"color", text.getColor()},
				{"width", tb.textBox->getWidth()},
				{"height", tb.textBox->getHeight()},
				{"x", tb.xPos},
				{"y", tb.yPos}
			});
		}

		slide["images"] = json::array();
		for (const auto &img : layout->getImages()) {
			slide["images"].push_back({
				{"url", img.image->getURL()},
				{"width", img.image->getWidth()},
				{"height", img.image->getHeight()},
				{"x", img.xPos},
				{"y", img.yPos}
			});
		}

		deck["slides"].push_back(slide);
	}

	exportedJson = deck.dump(2); // pretty print
	return exportedJson.c_str();
}
void call_addSlideChangeEvent() { PRESENTATION_MANAGER.addSlideChangeEvent(10, PRESENTATION_MANAGER.getCurrentPos(), PRESENTATION_MANAGER.getCurrentPos()+1); }
}
