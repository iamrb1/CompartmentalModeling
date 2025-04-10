window.addEventListener("DOMContentLoaded", () => {
  Module().then((instance) => {
    // Create a single "app state" object
    const app = new instance.RichTextState();

    // Grab references to each tab content area
    const htmlContainer      = document.getElementById("tab-html");
    const mdCodeElement      = document.getElementById("md-code");
    const rtfCodeElement     = document.getElementById("rtf-code");
    const rawHTMLCodeElement = document.getElementById("raw-html-code");

    // Function to update serialized outputs based on current state
    const updateOutputs = () => {
      htmlContainer.innerHTML = app.getHTML();
      mdCodeElement.textContent = app.getMarkdown();
      rtfCodeElement.textContent = app.getRTF();
      rawHTMLCodeElement.textContent = app.getRawHTML();
    };

    // Initial update of outputs
    updateOutputs();

    // Tab switching logic remains unchanged.
    const tabButtons  = document.querySelectorAll("#tab-buttons button");
    const tabContents = document.querySelectorAll(".tab-content");
    tabContents.forEach(div => div.classList.remove("active"));
    document.getElementById("tab-html").classList.add("active");

    tabButtons.forEach(btn => {
      btn.addEventListener("click", () => {
        tabContents.forEach(div => div.classList.remove("active"));
        const targetId = btn.getAttribute("data-tab-target");
        document.getElementById(targetId).classList.add("active");
      });
    });

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
      app.undo();
      updateOutputs();
    });
    
    redoButton.addEventListener("click", () => {
      app.redo();
      updateOutputs();
    });
    
    // Future: bind text input's events to update the state.
  });
});
