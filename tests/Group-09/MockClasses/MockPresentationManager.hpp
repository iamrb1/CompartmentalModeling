/**
 * @file MockPresentationManager.hpp
 * @brief This file contains PresentationManager with removed JS calls
 * @author Owen Haiar, Mary Holt, Allie Ianello, Grace Fitzgerald
 */

#pragma once



//#include <nlohmann/json.hpp>
#include "MockWebLayout.hpp"

using namespace cse;
//using json = nlohmann::json;
using Slide = std::shared_ptr<MockWebLayout>;

/**
 * Main manager for Liskov presentation program
 * Initialized globally, and update() is called in a loop while running
 */
class MockPresentationManager {
	private:
//		PresentationEventManager _event_manager;
		bool _running = false;
		int _current_pos = 0;
		std::vector<Slide> _slide_deck;

	public:

		/**
		 * Go to the first slide, stop events, and set the eventmanager size
		 */
		void initialize() {
			goTo(0);
			stop();
//			_event_manager.resize(_slide_deck.size());
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
			//_evemt_manager.resize(0);
			_current_pos = 0;
			//_event_manager.onSlideChanged(_current_pos);
		}

		/**
		 * Start the events
		 */
		void start() {
			//_event_manager.start();
            _running = true;
			std::cout << "Starting Events" << std::endl;
		}

		/**
		 * Stop the events
		 */
		void stop() {
			//_event_manager.stop();
            _running = false;
            std::cout << "Stopping Events" << std::endl;
		}

		/**
		 * Main driver for timing
		 */
		void update() { //_event_manager.update();
        }

		/**
		 * Add a new slide to the deck, and go to it
		 */
		void addNewSlide() {
			auto new_slide = std::make_shared<MockWebLayout>();
			_slide_deck.push_back(new_slide);
			//_event_manager.resize(_slide_deck.size()); // Update the event manager
			goTo(_slide_deck.size() - 1); // Go to the new slide
			std::cout << "Added new slide. ID: " << new_slide->getID() << std::endl;
		}

		/**
		 * Removes a slide
		 * @param slide
		 */
		void deleteSlide(const Slide& slide) {
			slide->deactivateLayout();
			std::erase(_slide_deck, slide);
			//_event_manager.resize(_slide_deck.size());
			std::cout << "Added new slide. ID: " << slide->getID() << std::endl;
		}

		/**
		 * Add a given slide to end of the deck
		 */
		void pushBackSlide(const Slide& slide) {
			_slide_deck.push_back(slide);
			//_event_manager.resize(_slide_deck.size());
			std::cout << "Pushed slide: " << slide->getID() << std::endl;
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
			cse::FormattedText ft;
			ft.setText("New Box!");
			TextBoxConfig tbc;
			tbc.content = ft;
			tbc.height = 10;
			tbc.width = 30;
			auto newTextBox = std::make_shared<TextBox>("", tbc);
			TextBoxLayout newLayout = {newTextBox, 50, 50};

			if (_slide_deck.empty() || _current_pos >= static_cast<int>(_slide_deck.size())) {
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

			if (_slide_deck.empty() || _current_pos >= static_cast<int>(_slide_deck.size())) {
				std::cout << "ERROR: No current layout to add image.\n";
                // Create new slide and finish adding image
                addNewSlide();
			}

			auto image = std::make_shared<MockImage>(url, width, height, altText);
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

			if (slide_num < 0 || slide_num >= static_cast<int>(_slide_deck.size())) {
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
			//_event_manager.onSlideChanged(_current_pos);
		}

		/**
		 * Updates object position
		 */
		void updatePosition(std::string id, int newX, int newY) {
			if (_slide_deck.empty() || _current_pos >= static_cast<int>(_slide_deck.size())) return;
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
		const std::vector<std::shared_ptr<MockWebLayout> > &getSlides() const {
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
		 * Add a slide change transition
		 * @param time in seconds
		 * @param origin Origin slide
		 * @param destination Destination slide
		 */
		void addSlideChangeEvent([[maybe_unused]]const int time, [[maybe_unused]]const size_t origin, const size_t destination) {
            // Check if destination slide exists (destination count is 1 off for user readability on UI)
            if(destination > static_cast<size_t>(getNumSlides() - 1)) {
              std::cout << "WARNING: Destination slide " << destination << " doesn't exist." << std::endl;
              return;
            }

			//_event_manager.addEvent(changeSlide, origin, destination, time);
		}

        void updateImageSize(const std::string& id, int newWidth, int newHeight) {
		  if (_slide_deck.empty() || _current_pos >= static_cast<int>(_slide_deck.size())) return;
		  auto& layout = _slide_deck.at(_current_pos);

		  for (auto& img : layout->getImages()) {
		    if (img.image->getID() == id) {
		      img.image->resize(newWidth, newHeight);
		      break;
		    }
		  }
		}

        bool isRunning() const { return _running; }

        void moveSlide(bool const forward) {

          // Check if any slides exist
          if(getNumSlides() == 0) {
            std::cout << "WARNING: No slides exist" << std::endl;
            return;
          }

          int destination = 0;

          if(forward) {
            // Check if is last slide
            if(_current_pos == getNumSlides() - 1) {
              std::cout << "WARNING: Already is last slide" << std::endl;
              return;
            }
            // Move forward one slide
            destination = _current_pos + 1;

          } else {
            // Check if is first slide
            if(_current_pos ==  0) {
              std::cout << "WARNING: Already is first slide" << std::endl;
              return;
            }

            // Move back one slide
            destination = _current_pos - 1;
          }

          // Swap position
          std::swap(_slide_deck[_current_pos], _slide_deck[destination]);
          std::cout << "Swapped slides " << _current_pos << " and " << destination << std::endl;
          goTo(destination);

        }

		////////////////////////////////////////////////////////////////////////////////////////
		/// STATIC EVENT FUNCTIONS
		/// These are passed to the Events to allow Events to change the layouts
		////////////////////////////////////////////////////////////////////////////////////////
		static void changeSlide(MockPresentationManager *presentation_manager, const int slideNum) {
			presentation_manager->goTo(slideNum);
		}
};

MockPresentationManager MOCK_PRESENTATION_MANAGER; ///< Global presentation manager



