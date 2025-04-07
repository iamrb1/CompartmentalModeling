/**
 * @file PresentationManager.h
 *
 * @author Owen Haiar
 */

#pragma once

#include "PresentationEventManager.hpp"
#include "../EventManager/EventManager.hpp"

using namespace cse;
// using LayoutManager = std::shared_ptr<WebLayoutManager>;
// using PresentationFunction = std::function<void(LayoutManager, size_t)>;

class PresentationManager {
	private:
		LayoutManager _layout_manager;
		PresentationEventManager _event_manager;
		bool _running = false;

	public:
		PresentationManager()
			: _layout_manager(std::make_shared<WebLayoutManager>()),
			  _event_manager(_layout_manager) {}

		void start() {
			_event_manager.start();
			EM_ASM({console.log("Starting Events"); });
		}
		void stop() {
			_event_manager.stop();
			EM_ASM({console.log("Pausing Events"); });
		}
		void update() { _event_manager.update(); }

		void changeSlideEvent(const int time, const size_t origin, const size_t destination) {
			_event_manager.addEvent(changeSlide, origin, destination, time);
		}

		// STATIC EVENT FUNCTIONS
		static void changeSlide(LayoutManager layoutManager,
		                        size_t slideNum) {
			layoutManager->goTo(slideNum);
		}
};

PresentationManager PRESENTATION_MANAGER;

extern "C" {
EMSCRIPTEN_KEEPALIVE
void pause() { PRESENTATION_MANAGER.stop(); }
void resume() { PRESENTATION_MANAGER.start(); }
}
