// Name of active tab
var last_active = null;
// Map storing output tab elements to swap between
var outputs = {};

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
    sum += i.innerText.length;
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

    // Left-side input bar as before.
    const bar = document.getElementById("toolbar");
    const wrappedInput = MakeInput.textInput("", "Type here...");
    wrappedInput.style.width = "100%";
    bar.appendChild(wrappedInput);

    // Create Undo and Redo buttons as proper <button> elements.
    const undoButton = document.createElement("button");
    undoButton.textContent = "Undo";
    // Add a class for styling.
    undoButton.className = "toolbar-button";
    
    const redoButton = document.createElement("button");
    redoButton.textContent = "Redo";
    redoButton.className = "toolbar-button";
    
    // Append the new buttons to the toolbar.
    bar.appendChild(undoButton);
    bar.appendChild(redoButton);
    
    // Hook up Undo/Redo handlers.
    undoButton.addEventListener("click", () => {
      // app.undo();
      updateOutputs();
    });
    
    redoButton.addEventListener("click", () => {
      // app.redo();
      updateOutputs();
    });
    
    // Future: bind text input's events to update the state.

    // Do funky stuff when selection changes
    document.addEventListener("selectionchange", () => {
      let e = document.getSelection();
      // If the selection is outside the editor, do nothing.
      if (!editorContainer.contains(e.anchorNode) || !editorContainer.contains(e.focusNode))
        return;

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
    });
  });
});
