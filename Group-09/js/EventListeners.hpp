/**
 * @file EventListeners.hpp
 *
 * @author Owen Haiar
 */

#pragma once
#include <emscripten/bind.h>

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
				function() {Module._call_rewind();});


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

        // Ability to hit ESC to leave present mode
        document.addEventListener("keydown", function(e) {
          if (e.key === "Escape") {
            Module.ccall(
                "call_leavePresentation",
                null,
                [],
                []
            );
          }
        });

        // Ability to hit space to skip to next slide
        document.addEventListener("keydown", function(e) {
          if (e.code === "Space") {
            Module.ccall(
                "call_nextEvent",
                null,
                [],
                []
            );
          }
        });

        // Check if item is being dragged
		document.addEventListener(
			"mousedown", function(e) {
			  // Check if clicked element is draggable
			  var draggable =
				  Module.ccall("call_isMoveableObject",  // C++ function name
							   "boolean",                // return type
							   ["string"],               // argument types
							   [e.target.id]             // arguments
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
			Module.ccall("call_updatePosition", null,
						 [ "string", "number", "number" ],
						 [ element.id.trim(), newX, newY ]);
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
