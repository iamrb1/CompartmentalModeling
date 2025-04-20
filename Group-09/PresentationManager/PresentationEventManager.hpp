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

using namespace cse;
using Event_T = Event<PresentationManager *, size_t>;
using PresentationFunction = std::function<void(PresentationManager *, size_t)>;

class PresentationEventManager {
	private:
		PresentationManager *_presentation_manager;
		int _id = 0; ///< ID tracker for Events
		bool _running = false;
		double _slide_elapsed_time = 0; ///< Time elapsed on current slide in seconds
		double _last_recorded_time = 0; ///< Last recorded time
		size_t _current_slide = 0; ///< Current slide being presented
		std::vector<EventQueue<PresentationManager *, size_t> > _queues; ///< Vector of EventQueues where _queues[x] corresponds to slide x
		EventQueue<PresentationManager *, size_t> _active_queue; ///< Copy of queue currently active, this is where events are removed from
		std::map<size_t, size_t> _id_to_slide; ///< EventID to Slide Num map, should speed up remove

		void _update_clock() {
			const double current_time = emscripten_get_now() / 1000.0;
			if (_running) _slide_elapsed_time += current_time - _last_recorded_time;
			_last_recorded_time = current_time;
		}

	public:
		explicit PresentationEventManager(PresentationManager *layout_manager) : _presentation_manager(layout_manager) {
		}

		void resize(const size_t n) {
			_queues.resize(n);
		}

		void clear() {
			_queues.clear();
			_id_to_slide.clear();
			_queues.resize(1);
		}

		void start() {
			_running = true;
			_slide_elapsed_time = 0.0;
			_active_queue = _queues[_current_slide];
		}

		void stop() {
			_running = false;
			_slide_elapsed_time = 0.0;
			_active_queue = _queues[_current_slide];
		}

		void update() {
			_update_clock();

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
		 * Change context to current slide
		 * @param slide_num
		 */
		void onSlideChanged(const size_t slide_num) {
			_current_slide = slide_num;
			_slide_elapsed_time = 0.0;
			_active_queue = _queues[slide_num];
			std::cout << "Managing Events on slide:  " << (slide_num + 1) << std::endl;
		}

		int addEvent(PresentationFunction function, const size_t origin, const size_t target, const int time) {
			if (target >= _queues.size()) {
				_queues.resize(target + 1);
			}
			const int id = _id++; // Increment ID
			_id_to_slide[id] = origin; // Map ID to slide num
			const auto event = Event(id, time, std::move(function), _presentation_manager, target);
			_queues[origin].add(event);
			_active_queue = _queues[_current_slide]; // Re-copy the active queue
			return event.getID();
		}

		void removeEvent(const int id) {
			auto event = _queues[_id_to_slide[id]].remove(id);
			assert(event.has_value());
			_id_to_slide.erase(id);
			_active_queue = _queues[_current_slide]; // Re-copy the active queue
		}

};
