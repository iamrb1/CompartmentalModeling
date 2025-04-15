document.addEventListener("DOMContentLoaded", () => {
    document.addEventListener("dblclick", function (e) {
      const target = e.target;
  
      if (target.classList.contains("textbox")) {
        target.setAttribute("contenteditable", "true");
        target.focus();
  
        const saveAndExit = () => {
          target.removeAttribute("contenteditable");
  
          const id = target.id;
          const updatedText = target.textContent;
  
          // Call into WebAssembly to update the text box content
          const updateTextInCpp = Module.cwrap('call_updateTextBoxContent', null, ['string', 'string']);
          updateTextInCpp(id, updatedText);
        };
  
        target.addEventListener("blur", saveAndExit, { once: true });
  
        target.addEventListener("keydown", function onKey(e) {
          if (e.key === "Escape") {
            target.removeEventListener("keydown", onKey);
            target.blur();
          }
        });
      }
    });

  // Resize Image button
  const resizeBtn = document.getElementById("resizeImageButton");
  if (resizeBtn) {
    resizeBtn.addEventListener("click", () => {
      const imageID = prompt("Enter image ID to resize:");
      const newWidth = parseInt(prompt("Enter new width:"));
      const newHeight = parseInt(prompt("Enter new height:"));

      if (!imageID || isNaN(newWidth) || isNaN(newHeight)) {
        alert("Invalid input.");
        return;
      }

      const imageEl = document.getElementById(imageID);
      if (!imageEl) {
        alert("Image not found!");
        return;
      }

      imageEl.style.width = `${newWidth}vw`;
      imageEl.style.height = `${newHeight}vh`;

      const updateImageSizeInCpp = Module.cwrap("call_updateImageSize", null, ["string", "number", "number"]);
      updateImageSizeInCpp(imageID, newWidth, newHeight);
    });
  }
});

// maybe we want to select image for resizing using a dropdown?
// const dropdown = document.getElementById("imageSelectDropdown");
// const allImages = document.querySelectorAll("img");
//
// allImages.forEach((img) => {
//   const id = img.id || "(no id)";
//   const option = document.createElement("option");
//   option.value = id;
//   option.textContent = `${id}`;
//   dropdown.appendChild(option);
// });
  