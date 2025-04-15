/**
 * @file PresentationManager.h
 * @brief This file contains the PresentationManager and the exported functions called by JS
 * @author Owen Haiar, Mary Holt, Allie Ianello
 */

#pragma once

#include <nlohmann/json.hpp>

#include "PresentationEventManager.hpp"
#include "../WebLayout/WebLayout.hpp"
#include "../js/EventListeners.hpp"

using namespace cse;
using json = nlohmann::json;
using Slide = std::shared_ptr<WebLayout>;

/**
 * Main manager for Liskov presentation program
 * Initialized globally, and update() is called in a loop while running
 */
class PresentationManager {
	private:
		PresentationEventManager _event_manager;
		bool _running = false;
		int _current_pos = 0;
		std::vector<Slide> _slide_deck;

	public:

		/**
		 * Construct, set the pointer for the eventmanager, bind js buttons
		 */
		PresentationManager() : _event_manager(this) {
			bind();
		}

		/**
		 * Go to the first slide, stop events, and set the eventmanager size
		 */
		void initialize() {
			goTo(0);
			stop();
			_event_manager.resize(_slide_deck.size());
			onSlideChangedJS();
		}

		/**
		 * Clear the slides and event manager
		 */
		void clear() {
			std::cout << "Clearing layouts" << std::endl;
			for (auto &layout : _slide_deck) {
				layout->deactivateLayout(); // Deactivate all layouts
			}
			_slide_deck.clear();
			_event_manager.resize(0);
			_current_pos = 0;
			_event_manager.onSlideChanged(_current_pos);
			onSlideChangedJS();
		}

		/**
		 * Start the events
		 */
		void start() {
			_event_manager.start();
            _running = true;
			std::cout << "Starting Events" << std::endl;
            // Toggle bottom bar
            toggleBottomNav(true);
		}

		/**
		 * Stop the events
		 */
		void stop() {
			_event_manager.stop();
            _running = false;
            std::cout << "Stopping Events" << std::endl;
		}

		/**
		 * Main driver for timing
		 */
		void update() { _event_manager.update(); }

		/**
		 * Add a new slide to the deck, and go to it
		 */
		void addNewSlide() {
			auto new_slide = std::make_shared<WebLayout>();
			_slide_deck.push_back(new_slide);
			_event_manager.resize(_slide_deck.size()); // Update the event manager
			goTo(_slide_deck.size() - 1); // Go to the new slide
			std::cout << "Added new slide. ID: " << new_slide->getID() << std::endl;
			onSlideChangedJS();
		}

		/**
		 * Removes a slide
		 * @param slide
		 */
		void deleteSlide(const Slide& slide) {
			slide->deactivateLayout();
			std::erase(_slide_deck, slide);
			_event_manager.resize(_slide_deck.size());
			std::cout << "Added new slide. ID: " << slide->getID() << std::endl;
			onSlideChangedJS();
		}

		/**
		 * Add a given slide to end of the deck
		 */
		void pushBackSlide(const Slide& slide) {
			_slide_deck.push_back(slide);
			_event_manager.resize(_slide_deck.size());
			std::cout << "Pushed slide: " << slide->getID() << std::endl;
			onSlideChangedJS();
		}

		/**
		 * Advances to the next slide
		 */
		void advance() { goTo(_current_pos + 1); }

		/**
		 * Rewinds to the previous slide
		 */
		void rewind() { goTo(_current_pos - 1); }

		/**
		 * Add a new text box to the current slide
		 */
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

			if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) {
				std::cout << "ERROR: No current layout to modify.\n";
                // Create new slide and finish adding textbox
                addNewSlide();
			}

			_slide_deck.at(_current_pos)->addTextBox(newLayout);
			_slide_deck.at(_current_pos)->activateLayout();
		}

		/**
		 * Add a new image to the current slide
		 */
		void addImage(const std::string &url,
		              int width,
		              int height,
		              const std::string &altText) {
			if (width <= 0 || height <= 0) {
				std::cout << "ERROR: Invalid image dimensions: width=" << width
						<< ", height=" << height << std::endl;
				return;
			}

			if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) {
				std::cout << "ERROR: No current layout to add image.\n";
                // Create new slide and finish adding image
                addNewSlide();
			}

			auto image = std::make_shared<Image>(url, width, height, altText);
			ImageLayout layout{image, 10, 10};
			_slide_deck.at(_current_pos)->addImage(layout);
			_slide_deck.at(_current_pos)->activateLayout();
		}

		/**
		 * Go to a given slide
		 * @param slide_num
		 */
		void goTo(const int slide_num) {
			std::cout << "Going to slide " << slide_num << std::endl;

			if (slide_num < 0 || slide_num >= _slide_deck.size()) {
				std::cout << "ERROR: Invalid slide number: " << slide_num << std::endl;
				return; // Exit early
			}

			auto currentLayout = _slide_deck.at(_current_pos);
			if (currentLayout) {
				currentLayout->deactivateLayout();
			}

			_current_pos = slide_num;

			currentLayout = _slide_deck.at(_current_pos);
			currentLayout->activateLayout();
			_event_manager.onSlideChanged(_current_pos);
			onSlideChangedJS();
		}

		/**
		 * Updates object position
		 */
		void updatePosition(std::string id, int newX, int newY) {
			if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) return;
			_slide_deck.at(_current_pos)->setPosition(id, newX, newY);
		}

		/**
		 * Returns if id is a moveable item
		 */
		bool isMoveableObject(std::string id) const {
			if (_slide_deck.empty()) return false;
			return _slide_deck.at(_current_pos)->contains(id);
		}

		/**
		 * Getter for slides
		 * @return slides in the deck
		 */
		const std::vector<std::shared_ptr<WebLayout> > &getSlides() const {
			return _slide_deck;
		}

		/**
		 * @brief Get the Current Position of the manager
		 * @return int current position in the slide vector
		 */
		int getCurrentPos() const { return _current_pos; }

		/**
		 * @brief Get the Current Position of the manager
		 * @return int current position in the slide vector
		 */
		int getNumSlides() const { return static_cast<int>(_slide_deck.size()); }

		/**
		 * Update html when number of slides or position has changed
		 */
		void onSlideChangedJS() {
			EM_ASM({
			const currentSlide = $0;
			const totalSlides = $1;
			document.getElementById("slideNumberDisplay").textContent =
				`Slide ${currentSlide} of ${totalSlides}`;
			}, getCurrentPos() + 1, getNumSlides());
		}

		/**
		 * Add a slide change transition
		 * @param time in seconds
		 * @param origin Origin slide
		 * @param destination Destination slide
		 */
		void addSlideChangeEvent(const int time, const size_t origin, const size_t destination) {
            // Check if destination slide exists (destination count is 1 off for user readability on UI)
            if(destination > getNumSlides() - 1) {
              std::cout << "WARNING: Destination slide " << destination << " doesn't exist." << std::endl;
              return;
            }

			_event_manager.addEvent(changeSlide, origin, destination, time);
		}

        void updateImageSize(const std::string& id, int newWidth, int newHeight) {
		  if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) return;
		  auto& layout = _slide_deck.at(_current_pos);

		  for (auto& img : layout->getImages()) {
		    if (img.image->getID() == id) {
		      img.image->resize(newWidth, newHeight);
		      break;
		    }
		  }
		}

        void toggleBottomNav(bool hidden) {
            // Hide bottom edit bar
            EM_ASM({
                var bottomNav = document.getElementById("bottom-nav");
                if(bottomNav){
                  console.log('Present Mode: ', $0);
                  bottomNav.style.visibility = $0 ? "hidden" : "visible";
                }
            }, hidden);
        }

        bool isRunning() const { return _running; }

		////////////////////////////////////////////////////////////////////////////////////////
		/// STATIC EVENT FUNCTIONS
		/// These are passed to the Events to allow Events to change the layouts
		////////////////////////////////////////////////////////////////////////////////////////
		static void changeSlide(PresentationManager *presentation_manager, const int slideNum) {
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

	if (PRESENTATION_MANAGER.getSlides().empty() ||
		PRESENTATION_MANAGER.getCurrentPos() >= PRESENTATION_MANAGER.getSlides().size()) {
		std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
		return;
		}

	auto& layout = PRESENTATION_MANAGER.getSlides().at(PRESENTATION_MANAGER.getCurrentPos());
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
bool call_isMoveableObject(const char* id) {
	std::string cppId(id);
	return PRESENTATION_MANAGER.isMoveableObject(id);
}
void loadSlideDeckFromJson(const char *jsonStr) {
	PRESENTATION_MANAGER.clear(); // Clear the deck before loading new slides
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
		PRESENTATION_MANAGER.pushBackSlide(layout); // Add to the global manager
	}
	PRESENTATION_MANAGER.initialize(); // Show the first slide
}
const char *exportSlideDeckToJson() {
	static std::string exportedJson;
	json deck;
	deck["slides"] = json::array();

	for (const auto &layout : PRESENTATION_MANAGER.getSlides()) {
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

void call_updateImageSize(const char* id, int width, int height) { std::string cppId(id); PRESENTATION_MANAGER.updateImageSize(cppId, width, height);}

void call_leavePresentation() {
  PRESENTATION_MANAGER.toggleBottomNav(false);
  PRESENTATION_MANAGER.stop();
}

void call_nextEvent() {
  if(PRESENTATION_MANAGER.isRunning()) {
     PRESENTATION_MANAGER.advance();
  }
}


}

