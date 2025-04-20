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

    var moveForwardButton = document.getElementById("moveForwardButton");
    if (moveForwardButton) {
          moveForwardButton.addEventListener(
              "click",
              function() {Module._call_moveSlide(true); });
        }

    var moveBackButton = document.getElementById("moveBackButton");
        if (moveBackButton) {
          moveBackButton.addEventListener(
              "click",
              function() {Module._call_moveSlide(false); });
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
				function() {
				const input = prompt("Transition after:");
				const value = parseInt(input, 10);
				Module.ccall("call_addSlideChangeEvent", null,
							 [ "number" ],
							 [ value ]);
			});
		}

        // Ability to hit ESC to leave present mode
        document.addEventListener("keydown", function(e) {
          if (e.key === "Escape") {
          	e.preventDefault();
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

		// Right click to animate objects
		document.addEventListener("contextmenu", function(e) {
		var draggable =
			  Module.ccall("call_isMoveableObject",  // C++ function name
						   "boolean",                // return type
						   ["string"],               // argument types
						   [e.target.id]             // arguments
			  );

		  if (draggable) {
		  	e.preventDefault();
			animateObject(e.target);
		  }
		});

		// Add an event to the right clicked object
		function animateObject(element) {
			const slideNum = Module.ccall(
				"call_getCurrentPos",
				"number",
				[],
				[]
			);

			const input = prompt("Appear after:");
			const value = parseInt(input, 10);
			Module.ccall("call_addObjectEvent", null,
						 [ "number", "number", "string" ],
						 [ value, slideNum, element.id.trim() ]);
			};

        // Check if item is being dragged
		document.addEventListener(
			"mousedown", function(e) {
			  // Check if clicked element is draggable
			  var draggable =
				  Module.ccall("call_isMoveableObject",
							   "boolean",
							   ["string"],
							   [e.target.id]
				  );

			  if (draggable) {
				makeDraggable(e.target, e);
			  }
			});

		// Function to make an element draggable
        function makeDraggable(element, event) {
          //Ensure images aren't default dragging - ChatGPT
          element.ondragstart = () => false;

          // Toggle for if dragging or resizing
          var dragging = true;

          //Calculate mouse offset
          var box = element.getBoundingClientRect();
          var offsetX = (event.clientX - box.left) / window.innerWidth * 100;
          var offsetY = (event.clientY - box.top) / window.innerHeight * 100;

          // Calculate bottom-right corner of resizing area
          var resizeOffset = 16;
          var xCorner = box.left + box.width - resizeOffset;
          var yCorner = box.top + box.height - resizeOffset;

          if(event.clientX >= xCorner && event.clientY >= yCorner) {
            dragging = false;
          }

		  // ChatGPT used for new position calculations
		  document.onmousemove = function(e) {
            if(dragging) {
              var newX = (e.clientX / window.innerWidth) * 100 - offsetX;
              var newY = (e.clientY / window.innerHeight) * 100 - offsetY;

              // Call C++ function to update position within the layout
              Module.ccall("call_updatePosition", null,
              ["string", "number", "number"],
              [ element.id.trim(), newX, newY ]);
              // Update element's visual position
              element.style.left = newX + "vw";
              element.style.top = newY + "vh";
            } else {
              var newWidth = ((e.clientX - box.left) / window.innerWidth) * 100;
              var newHeight = ((e.clientY - box.top) / window.innerHeight) * 100;

              // Call C++ function to update size within the layout
              Module.ccall("call_updateSize", null,
              ["string", "number", "number"],
              [ element.id.trim(), newWidth, newHeight ]);
              // Update element's visual size
              element.style.width = newWidth + "vw";
              element.style.height = newHeight + "vh";
            }
		  };

		  document.onmouseup = function(e) {
			// Remove event listeners
			document.onmousemove = null;
			document.onmouseup = null;
		  };
		}
	});
}
