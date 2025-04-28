/**
 * @file PresentationEventManager.hpp
 *
 * @author Owen Haiar
 */

#pragma once

#include <emscripten.h>
#include <map>
#include <vector>
#include "../EventQueue/EventQueue.hpp"

class PresentationManager; ///< Forward declaration so Events can store a pointer to the PresentationManager

/**
 * Types of Events we store
 */
enum class EventType {
	Image,
	Textbox,
	Slide,
	UNKNOWN
};

/**
 * Object IDs look like "textbox-n", "image-n", or "n" for textboxes, images, and slides respectively
 * @param object_id
 * @return EventType of object
 */
inline EventType parseType(const std::string &object_id) {
	size_t dashpos = object_id.find('-');
	if (dashpos == std::string::npos) {
		try {
			stoi(object_id); // Check if int
			return EventType::Slide;
		} catch (...) {
			return EventType::UNKNOWN;
		}
	}
	const std::string prefix = object_id.substr(0, dashpos);
	auto type = EventType::UNKNOWN;
	if (prefix == "textbox") {
		type = EventType::Textbox;
	} else if (prefix == "image") {
		type = EventType::Image;
	}
	return type;
}

/**
 * Parse the numerical id from a string id
 * @param object_id
 * @return
 */
inline size_t parseID(const std::string &object_id) {
	size_t dashpos = object_id.find('-');
	if (dashpos == std::string::npos) {
		return stoi(object_id);
	}
	const std::string id = object_id.substr(dashpos + 1);
	return stoi(id);
}

using namespace cse;
using PresentationFunction = std::function<void(PresentationManager *, size_t)>;

/**
 * Event manager for presentation project
 */
class PresentationEventManager {

	private:
		PresentationManager *_presentation_manager;
		bool _running = false;
		double _slide_elapsed_time = 0; ///< Time elapsed on current slide in seconds
		double _last_recorded_time = 0; ///< Last recorded time
		size_t _current_slide = 0; ///< Current slide being presented
		std::vector<EventQueue<PresentationManager *, size_t> > _queues;
		///< Vector of EventQueues where _queues[x] corresponds to slide x
		EventQueue<PresentationManager *, size_t> _active_queue;
		///< Copy of queue currently active, this is where events are popped and executed from
		std::map<std::string, size_t> _id_to_slide; ///< EventID to Slide Num map, should speed up remove

		/**
		 * Update the internal clock, if running
		 */
		void _updateClock() {
			const double current_time = emscripten_get_now() / 1000.0;
			if (_running) _slide_elapsed_time += current_time - _last_recorded_time;
			_last_recorded_time = current_time;
		}

		/**
		 * Force all events to execute
		 */
		void _exitEarly() {
			while (_active_queue.size() > 0) {
				auto e = _active_queue.pop();
				if (const auto type = parseType(e.getID());
					type == EventType::Image || type == EventType::Textbox) e.execute();
			}
		}

	public:
		explicit PresentationEventManager(PresentationManager *layout_manager) : _presentation_manager(layout_manager) {
		}

		/**
		 * Resize the number of managed slides
		 * @param n
		 * @param shrink
		 */
		void resize(const size_t n, const bool shrink = false) {
			if (!shrink && _queues.size() >= n) return;
			_queues.resize(n);
		}

		/**
		 * This handles the edge case where a slide transition occurs while there are remaining object events
		 * The object still needs to reappear in the layout
		 */
		void resetObjects() {
			_exitEarly();
		}

		/**
		 * Clear the EventManager
		 */
		void clear() {
			_id_to_slide.clear();
			_queues.clear();
			_queues.resize(1);
		}

		/**
		 * Start clock on current slide and execute initial events
		 */
		void start() {
			assert(_current_slide < _queues.size());
			_running = true;
			_slide_elapsed_time = 0.0;
			_active_queue = _queues[_current_slide];
		}

		/**
		 * Stop clock
		 */
		void stop() {
			_exitEarly();
			_running = false;
			_slide_elapsed_time = 0.0;
		}

		/**
		 * Advance to the next event. Return false if no events are left
		 */
		bool next() {
			if (_active_queue.size() == 0) return false;
			_slide_elapsed_time = _active_queue.peek().getTime();
			return true;
		}

		/**
		 * Update clock and execute pending events
		 */
		void update() {
			_updateClock();

			// Return if stopped
			if (!_running) return;

			// Check if slide is valid
			if (_current_slide >= _queues.size()) {
				std::cout << "Events: Slide " << (_current_slide + 1) << " does not exist." << std::endl;
				return;
			}

			// Check queue and execute
			while (_active_queue.size() && _slide_elapsed_time >= _active_queue.peek().getTime()) {
				_active_queue.pop().execute();
			}
		}

		/**
		 * Delete a slide from the EventManager
		 */
		void deleteSlide(const int slide_num) {
			while (_queues[slide_num].size() > 0) {
				auto e = _active_queue.pop();
				_id_to_slide.erase(e.getID());
			}
			_queues.erase(_queues.begin() + slide_num);
		}

		/**
		 * Change context to current slide
		 * @param slide_num
		 */
		void onSlideChanged(const size_t slide_num) {
			_current_slide = slide_num;
			_slide_elapsed_time = 0.0;
			_active_queue = _queues[slide_num];
			std::cout << "Managing Events on slide:  " << (slide_num + 1) << std::endl;
		}

		/**
		 * Add an event to the manager
		 * @param function
		 * @param origin
		 * @param target
		 * @param time
		 */
		void addEvent(const PresentationFunction &function,
		              const size_t origin,
		              const std::string &target,
		              const int time) {
			if (parseType(target) == EventType::Slide) {
				// Assert we are not transitioning out of bounds
				assert(origin + 1 < _queues.size());
			} else if (parseType(target) == EventType::Image || parseType(target) == EventType::Textbox) {
				// Add an event to toggle initially toggle the object off
				const auto event = Event(target + "-rm", 0, function, _presentation_manager, parseID(target));
				_queues[origin].add(event);
			}

			const auto event = Event(target, time, function, _presentation_manager, parseID(target));
			try {
				_queues[origin].add(event);
				_id_to_slide[target] = origin; // Map ID to slide num
			} catch (const std::invalid_argument &e) {
				std::cout << "Event " << target << " already has an event." << std::endl;
			}
			_active_queue = _queues[_current_slide]; // Re-copy the active queue
		}

		/**
		 * Delete an event by ID
		 * @param id
		 */
		void removeEvent(const std::string &id) {
			if (parseType(id) == EventType::Image || parseType(id) == EventType::Textbox) {
				// Remove the toggle off event
				const auto event = _queues[_id_to_slide[id]].remove(id + "-rm");
			}
			// Remove the event
			const auto event = _queues[_id_to_slide[id]].remove(id);
			_id_to_slide.erase(id);
			_active_queue = _queues[_current_slide]; // Re-copy the active queue
			if (event.has_value()) std::cout << "Removing event for " << id << std::endl;
		}

		/**
		 * Return event info on a given slide
		 * @param slideNum
		 * @return map of IDs to timings
		 */
		std::unordered_map<std::string, int> getSlideEventInfo(const int slideNum) const {
			std::unordered_map<std::string, int> map{};
			auto queue_copy = _queues[slideNum];
			while (queue_copy.size() > 0) {
				const auto event = queue_copy.pop();
				map[event.getID()] = event.getTime();
			}
			return map;
		}
};
