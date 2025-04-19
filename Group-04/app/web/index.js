// Name of active tab
var last_active = null;
// Map storing output tab elements to swap between
var outputs = {};
// Track whether we are selecting text
var selecting = false;

// Convert an emscripten vector to a JS array
function convert_vector(v) {
  let out = [];
  for(let i = 0; i < v.size(); i++)
  {
    out.push(v.get(i));
  }
  return out;
}

// Calculate offset of element within another element (text based)
// This is a first write of this function so there may be bugs
function calculate_offset(container, child, offset = 0) {
  let sum = 0;
  for (let i of container.childNodes) {
    if (i == child)
      return sum + offset;
    else if (i.contains(child))
      return sum + calculate_offset(i, child, offset);
    sum += i.textContent.length;
  }

  return sum;
}

window.addEventListener("DOMContentLoaded", () => {
  Module().then((instance) => {
    // Create a single "app state" object
    const app = new instance.RichTextState();

    // Output elements
    const outputContainer = document.getElementsByTagName("output")[0];
    const buttonContainer = document.getElementById("tab-buttons");
    const editorContainer = document.getElementById("text-editor");

    // Generate new tabs and buttons based on names
    // from editor
    const formats = [];
    let tmp_formats = app.output_formats();

    for(let i = 0; i < tmp_formats.size(); i++)
    {
      // Horrible no good very bad conversion from vectors to
      // JS arrays because emscripten hates us
      formats.push(convert_vector(tmp_formats.get(i)));
    }

    // A swapping function that lets us easily
    // change between tabs with minimal overhead
    let swap_to = (next) => {
      outputs[last_active].classList.remove("active");
      outputs[next].classList.add("active");
      last_active = next;
    };

    // Add all formats to the output element
    for(let i of formats) {
      // Tab to show output text
      let tab = document.createElement("div");
      tab.classList.add("tab-content");
      // Button to swap to this output
      let button = document.createElement("button");
      button.innerText = i[1];
      button.addEventListener("click", swap_to.bind(null, i[0]));

      outputs[i[0]] = tab;
      // Add the elements to the containers
      outputContainer.appendChild(tab);
      buttonContainer.appendChild(button);
    }

    // Show the first avalable output format
    last_active = formats[0][0];
    outputs[last_active].classList.add("active");

    // Function to update serialized outputs based on current state
    const updateOutputs = () => {
      editorContainer.innerHTML = app.output("html");

      for(let i in outputs) {
        let out = app.output(i);
        if (i == "html")
          outputs[i].innerHTML = out;
        else {
          while(outputs[i].firstElementChild)
            outputs[i].firstElementChild.remove();
          let pre = document.createElement("pre");
          let code = document.createElement("code");
          code.innerText = out;
          pre.appendChild(code);
          outputs[i].appendChild(pre);
        }
      }
    };

    // Initial update of outputs
    updateOutputs();

    // Left-side input bar
    const bar = document.getElementById("toolbar");
    const wrappedInput = MakeInput.textInput("", "Type here...");
    wrappedInput.style.width = "100%";
    bar.appendChild(wrappedInput);

    // Add some spacing
    bar.appendChild(document.createElement("br"));
    bar.appendChild(document.createElement("br"));

    // Create formatting buttons
    const formatButtonsContainer = document.createElement("div");
    formatButtonsContainer.className = "format-buttons";
    
    // Bold button
    const boldButton = document.createElement("button");
    boldButton.textContent = "B";
    boldButton.className = "format-button bold-button";
    boldButton.addEventListener("click", () => {
      app.applyBold();
      updateOutputs();
    });
    
    // Italic button
    const italicButton = document.createElement("button");
    italicButton.textContent = "I";
    italicButton.className = "format-button italic-button";
    italicButton.addEventListener("click", () => {
      app.applyItalic();
      updateOutputs();
    });
    
    // Underline button
    const underlineButton = document.createElement("button");
    underlineButton.textContent = "U";
    underlineButton.className = "format-button underline-button";
    underlineButton.addEventListener("click", () => {
      app.applyUnderline();
      updateOutputs();
    });
    
    // Color button
    const colorButton = document.createElement("button");
    colorButton.textContent = "A";
    colorButton.className = "format-button color-button";
    colorButton.addEventListener("click", () => {
      const color = document.getElementById("colorPicker").value;
      console.log("Applying color:", color); // Debug
      app.applyColor(color);
      updateOutputs();
    });
    
    // Color picker input
    const colorPicker = document.createElement("input");
    colorPicker.type = "color";
    colorPicker.id = "colorPicker";
    colorPicker.className = "color-picker";
    colorPicker.value = "#000000";
    
    // Add event listener to color picker to apply color directly when changed
    colorPicker.addEventListener("input", () => {
      const color = colorPicker.value;
      console.log("Color picked:", color);
      app.applyColor(color);
      updateOutputs();
      
      // Show a visual indicator that color was applied
      const colorMessage = document.createElement("div");
      colorMessage.textContent = `Color ${color} applied`;
      colorMessage.className = "operation-message color-message";
      colorMessage.style.backgroundColor = color;
      
      // Adjust text color based on background color brightness for readability
      const r = parseInt(color.slice(1, 3), 16);
      const g = parseInt(color.slice(3, 5), 16);
      const b = parseInt(color.slice(5, 7), 16);
      const brightness = (r * 299 + g * 587 + b * 114) / 1000;
      colorMessage.style.color = brightness > 125 ? 'black' : 'white';
      
      bar.appendChild(colorMessage);
      
      // Remove the message after a delay
      setTimeout(() => {
        if (colorMessage.parentNode) {
          colorMessage.parentNode.removeChild(colorMessage);
        }
      }, 2000);
    });
    
    // Append formatting buttons to container
    formatButtonsContainer.appendChild(boldButton);
    formatButtonsContainer.appendChild(italicButton);
    formatButtonsContainer.appendChild(underlineButton);
    formatButtonsContainer.appendChild(colorPicker);
    formatButtonsContainer.appendChild(colorButton);
    
    // Add spacing after format buttons
    bar.appendChild(formatButtonsContainer);
    bar.appendChild(document.createElement("br"));
    bar.appendChild(document.createElement("br"));
    
    // Custom format section
    const customFormatContainer = document.createElement("div");
    customFormatContainer.className = "custom-format-container";
    
    // Format input (text input instead of dropdown)
    const formatNameLabel = document.createElement("label");
    formatNameLabel.textContent = "Format:";
    formatNameLabel.htmlFor = "formatName";
    
    const formatNameInput = document.createElement("input");
    formatNameInput.type = "text";
    formatNameInput.id = "formatName";
    formatNameInput.className = "format-input";
    formatNameInput.placeholder = "bold, italic, color, etc.";
    
    // Format value input
    const formatValueContainer = document.createElement("div");
    formatValueContainer.id = "valueContainer";
    formatValueContainer.className = "format-value-container";
    
    const formatValueLabel = document.createElement("label");
    formatValueLabel.textContent = "Value:";
    formatValueLabel.htmlFor = "formatValue";
    
    const formatValueInput = document.createElement("input");
    formatValueInput.type = "text";
    formatValueInput.id = "formatValue";
    formatValueInput.className = "format-value-input";
    formatValueInput.placeholder = "Value (if needed)";
    
    formatValueContainer.appendChild(formatValueLabel);
    formatValueContainer.appendChild(formatValueInput);
    
    // Apply custom format button
    const applyFormatButton = document.createElement("button");
    applyFormatButton.textContent = "Apply";
    applyFormatButton.className = "apply-format-button";
    applyFormatButton.addEventListener("click", () => {
      const formatName = formatNameInput.value.trim().toLowerCase();
      const formatValue = formatValueInput.value.trim();
      
      console.log("Applying custom format:", formatName, formatValue);
      
      if (!formatName) {
        // Show error message if format name is empty
        const errorMessage = document.createElement("div");
        errorMessage.textContent = "Format name is required";
        errorMessage.className = "operation-message error-message";
        bar.appendChild(errorMessage);
        
        setTimeout(() => {
          if (errorMessage.parentNode) {
            errorMessage.parentNode.removeChild(errorMessage);
          }
        }, 2000);
        
        return;
      }
      
      try {
        // Handle different format types
        if (formatName === "bold") {
          app.applyBold();
        } 
        else if (formatName === "italic") {
          app.applyItalic();
        } 
        else if (formatName === "underline") {
          app.applyUnderline();
        } 
        else if (formatName === "strikethrough") {
          app.applyStrikethrough();
        } 
        else if (formatName === "color" && formatValue) {
          app.applyColor(formatValue);
        } 
        else if (formatName === "link" && formatValue) {
          app.applyLink(formatValue);
        } 
        else if (formatName === "header" && formatValue && !isNaN(parseInt(formatValue))) {
          app.applyHeader(parseInt(formatValue));
        } 
        else {
          // Attempt to apply a custom format with the given name and value
          // This allows for future extensibility even if we don't have a specific method
          console.log(`Applying custom format "${formatName}" with value "${formatValue}"`);
          
          // Here we need to infer the type based on the format name and value
          // For now, if value is empty, use monostate, otherwise use the string as is
          let success = false;
          
          if (!formatValue) {
            // No value, apply as a boolean flag format (like bold, italic)
            app.update_format(formatName, {});
            success = true;
          } else if (!isNaN(parseInt(formatValue))) {
            // Numeric value - try to apply as integer
            try {
              const numValue = parseInt(formatValue);
              app.update_format(formatName, numValue);
              success = true;
            } catch (e) {
              console.error("Failed to apply numeric format:", e);
              // Fall back to string format
              app.update_format(formatName, formatValue);
              success = true;
            }
          } else {
            // String value
            app.update_format(formatName, formatValue);
            success = true;
          }
          
          if (!success) {
            throw new Error(`Unknown format type: ${formatName}`);
          }
        }
        
        updateOutputs();
        
        // Show success message
        const successMessage = document.createElement("div");
        successMessage.textContent = `"${formatName}" formatting applied`;
        successMessage.className = "operation-message format-success-message";
        bar.appendChild(successMessage);
        
        setTimeout(() => {
          if (successMessage.parentNode) {
            successMessage.parentNode.removeChild(successMessage);
          }
        }, 2000);
      } catch (error) {
        console.error("Error applying format:", error);
        
        // Show error message
        const errorMessage = document.createElement("div");
        errorMessage.textContent = `Error applying "${formatName}": ${error.message}`;
        errorMessage.className = "operation-message error-message";
        bar.appendChild(errorMessage);
        
        setTimeout(() => {
          if (errorMessage.parentNode) {
            errorMessage.parentNode.removeChild(errorMessage);
          }
        }, 2000);
      }
    });
    
    // Append format elements to container
    const formatNameContainer = document.createElement("div");
    formatNameContainer.style.marginBottom = "10px";
    formatNameContainer.appendChild(formatNameLabel);
    formatNameContainer.appendChild(formatNameInput);
    
    customFormatContainer.appendChild(formatNameContainer);
    customFormatContainer.appendChild(formatValueContainer);
    customFormatContainer.appendChild(applyFormatButton);
    
    // Create Undo and Redo buttons
    const undoButton = document.createElement("button");
    undoButton.textContent = "Undo";
    undoButton.className = "toolbar-button";
    
    const redoButton = document.createElement("button");
    redoButton.textContent = "Redo";
    redoButton.className = "toolbar-button";
    
    // Append all UI elements to the toolbar
    bar.appendChild(customFormatContainer);
    bar.appendChild(document.createElement("br"));
    bar.appendChild(document.createElement("br"));
    bar.appendChild(undoButton);
    bar.appendChild(redoButton);
    
    // Hook up Undo/Redo handlers
    undoButton.addEventListener("click", () => {
      app.undo();
      updateOutputs();
      
      // Create and show a success message
      const undoMessage = document.createElement("div");
      undoMessage.textContent = "Undo applied successfully";
      undoMessage.className = "operation-message undo-message";
      bar.appendChild(undoMessage);
      
      // Remove the message after a delay
      setTimeout(() => {
        if (undoMessage.parentNode) {
          undoMessage.parentNode.removeChild(undoMessage);
        }
      }, 2000);
    });
    
    redoButton.addEventListener("click", () => {
      app.redo();
      updateOutputs();
      
      // Create and show a success message
      const redoMessage = document.createElement("div");
      redoMessage.textContent = "Redo applied successfully";
      redoMessage.className = "operation-message redo-message";
      bar.appendChild(redoMessage);
      
      // Remove the message after a delay
      setTimeout(() => {
        if (redoMessage.parentNode) {
          redoMessage.parentNode.removeChild(redoMessage);
        }
      }, 2000);
    });
    
    // Bind text input's events to update the state
    wrappedInput.addEventListener("input", (e) => {
      app.edit_change(wrappedInput.getValue());
      updateOutputs();
    });

    window.addEventListener("mousedown",  (e) => {
      if (editorContainer.contains(e.target))
        selecting = true;
    });

    const updateSelection = () => {
      // if we are not selecting then do nothing
      if (!selecting)
        return;

      let e = document.getSelection();

      let o1 = calculate_offset(editorContainer, e.anchorNode, e.anchorOffset);
      let o2 = calculate_offset(editorContainer, e.focusNode, e.focusOffset);
      if (o1 == o2) {
        let str = app.edit_start_pos(o1);
        wrappedInput.setValue(str);
      } else {
        let min = Math.min(o1, o2);
        let max = Math.max(o1, o2);
        let str = app.edit_start_range(min, max - min);
        wrappedInput.setValue(str);
      }
    };

    document.addEventListener("selectionchange", updateSelection);

    // Do funky stuff when selection changes
    window.addEventListener("mouseup", () => {
      updateSelection();
      selecting = false;
    });
  });
});
