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
#include "../WebLayout/WebLayoutManager.hpp"

using namespace cse;
using LayoutManager = std::shared_ptr<WebLayoutManager>;
using PresentationFunction = std::function<void(LayoutManager, size_t)>;

class PresentationEventManager {
	private:
		LayoutManager _layout_manager;
		int _id = 0; ///< ID tracker for Events
		bool _running = false;
		double _slide_elapsed_time = 0; ///< Time the current slide became active
		double _last_recorded_time = 0; ///< Current time
		size_t _current_slide = 0; ///< Current slide being presented
		std::vector<EventQueue<LayoutManager, size_t> > _queues;
		///< Vector of EventQueues where _queues[x] corresponds to slide x
		std::map<size_t, size_t> _id_to_slide; ///< EventID to Slide Num map, should speed up remove

		void _update_clock() {
			const double current_time = emscripten_get_now() / 1000.0;
			if (_running) _slide_elapsed_time += current_time - _last_recorded_time;
			_last_recorded_time = current_time;
		}

	public:
		explicit PresentationEventManager(LayoutManager layout_manager) : _layout_manager(std::move(layout_manager)) {
		}

		void start() { _running = true; }

		void stop() { _running = false; }

		void update() {
			_update_clock();

			// Return if stopped
			if (!_running) return;

			// Check if slide is valid
			if (_current_slide >= _queues.size()) {
				EM_ASM({console.log("EventManager is managing a non-existent slide"); });
				return;
			}

			auto active_queue = _queues[_current_slide];
			// Check queue and execute
			if (active_queue.size() && _slide_elapsed_time >= active_queue.peek().getTime()) {
				active_queue.pop().execute();
			}
		}

		void changeSlide(size_t slide_num) {
			_current_slide = slide_num;
			_slide_elapsed_time = 0.0;
		}

		int addEvent(PresentationFunction function, size_t origin, size_t destination, int time) {
			const int id = _id++; // Increment ID
			_id_to_slide[id] = origin; // Map ID to slide num
			auto event = Event(id, time, function, _layout_manager, destination);
			_queues[origin].add(event);
			return id;
		}

		void removeEvent(const int id) {
			auto event = _queues[_id_to_slide[id]].remove(id);
			assert(event.has_value());
			_id_to_slide.erase(id);
		}
};
