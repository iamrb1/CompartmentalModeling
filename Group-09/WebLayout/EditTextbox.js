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
          const updateTextInCpp = Module.cwrap('updateTextBoxContent', null, ['string', 'string']);
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
  });
  