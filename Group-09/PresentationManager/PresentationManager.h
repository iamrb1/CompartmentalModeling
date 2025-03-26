/**
 * @file PresentationManager.h
 *
 * @author Owen Haiar
 */

#pragma once

#include "../EventManager/EventManager.hpp"
#include "../WebLayout/WebLayoutManager.h"

using namespace cse;

class PresentationManager {

 public: // <- all pub for testing // TODO: change back to private

  /// private:

  // PresentationManager holds a WebLayoutManager and an EventManager.
  // The WebLayoutManager pointer is the first argument for the static functions that change the presentation and are called
  // inside of the events.
  std::shared_ptr<WebLayoutManager> layoutManager;
  EventManager<std::shared_ptr<WebLayoutManager>, size_t> eventManager;

  /// public:

  // A new PresentationManager creates a new WebLayoutManager
  PresentationManager() : layoutManager(std::make_shared<WebLayoutManager>()) {}

  // STATIC EVENT FUNCTIONS
  // We create static functions that can be passed to the EventManager (and the Event)
  // The first argument is a pointer to the layoutManager, the second is an ID (slide number, text box id, etc.)
  static void changeSlide(std::shared_ptr<WebLayoutManager> layoutManager, size_t slideNum) {
	layoutManager->advance();
  }
};

int main() {
  PresentationManager manager;
  // Here we add an event that advances to slide 5 after 1 second
  manager.eventManager.AddEvent(1, manager.changeSlide, manager.layoutManager, 5);
  return 0;
}

