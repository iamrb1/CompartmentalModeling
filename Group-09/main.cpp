/**
 * @file main.cpp
 *
 * @author Owen Haiar
 */

#include <emscripten.h>
#include "PresentationManager/PresentationManager.hpp"

constexpr int FPS = 5; ///< Update rate for main loop

using namespace cse;

/**
 * Main loop for emscripten
 * Required for timing based events
 */
void PresenterLoop() {
	PRESENTATION_MANAGER.update();
}

int main() {
	emscripten_set_main_loop(PresenterLoop, FPS, 1);
}
