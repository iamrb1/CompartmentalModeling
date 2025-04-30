// @file EditElement.js
//
// @author Allie Ianello & Mia Bodenhorn

document.addEventListener("DOMContentLoaded", () => {
  // True if formatting menu is open and textbox is editable, false otherwise
  let formattingLocked = false;

  // ID of the currently active textbox
  let currentTextboxId = null;

  // Open formatting menu and manage blur (deavtication)
  document.addEventListener("dblclick", function (e) {
    const target = e.target;

    if (target.classList.contains("textbox")) {
      currentTextboxId = target.id;
      target.setAttribute("contenteditable", "true");
      target.focus();

      showFormattingMenu(currentTextboxId);

      target.addEventListener("blur", () => {
        setTimeout(() => {
          if (!formattingLocked) {
            saveAndExit();
          } else {
            console.log("[blur] formattingLocked is true — skipping exit");
            target.focus();
          }
        }, 100);
      }, { once: true });
    }
  });

  // Prevent blur when clicking inside the formatting menu
  document.getElementById("formatting-menu").addEventListener("mousedown", () => {
    formattingLocked = true;
  });

  // Apply formatting
  document.getElementById("apply-formatting-btn").addEventListener("click", () => {
    applyFormatting();
  });

  // Cancel formatting
  document.getElementById("cancel-formatting-btn").addEventListener("click", () => {
    hideFormattingMenu();
  });

  // Show formatting menu at the correct location (above the textbox)
  function showFormattingMenu(textboxId) {
    formattingLocked = true;
    currentTextboxId = textboxId;
    const menu = document.getElementById("formatting-menu");
    const textbox = document.getElementById(textboxId);
    const rect = textbox.getBoundingClientRect();

    requestAnimationFrame(() => {
      const menuWidth = menu.offsetWidth;
      const menuHeight = menu.offsetHeight;

      let left = rect.left + rect.width / 2 - menuWidth / 2;
      let top = rect.top - menuHeight - 10;

      if (left < 0) left = 10;
      if (top < 0) top = rect.bottom + 10;

      menu.style.left = `${left}px`;
      menu.style.top = `${top}px`;
      menu.style.display = "block";
    });
  }

  function hideFormattingMenu() {
    formattingLocked = false;
    document.getElementById("formatting-menu").style.display = "none";
  }

  // Apply formatting to the textbox and update the backend
  function applyFormatting() {
    const font = document.getElementById("font-select").value;
    const fontSize = parseInt(document.getElementById("font-size-input").value);
    const color = document.getElementById("color-picker").value;

    if (!currentTextboxId) {
      console.warn("[applyFormatting] No current textbox selected.");
      return;
    }

    ccall('call_updateTextBoxFont', null, ['string', 'string'], [currentTextboxId, font]);
    ccall('call_updateTextBoxFontSize', null, ['string', 'number'], [currentTextboxId, fontSize]);
    ccall('call_updateTextBoxColor', null, ['string', 'string'], [currentTextboxId, color]);

    const textbox = document.getElementById(currentTextboxId);
    if (textbox) {
      textbox.style.fontFamily = font;
      textbox.style.fontSize = fontSize + "px";
      textbox.style.color = color;

      console.log("[applyFormatting] Updated DOM styles:", {
        fontFamily: textbox.style.fontFamily,
        fontSize: textbox.style.fontSize,
        color: textbox.style.color
      });
    }

    saveAndExit();
  }

  // Save changes and exit formatting mode
  function saveAndExit() {
    if (!currentTextboxId) return;
    const textbox = document.getElementById(currentTextboxId);
    if (!textbox) return;

    textbox.removeAttribute("contenteditable");

    const updatedText = textbox.textContent;
    const updateTextInCpp = Module.cwrap('call_updateTextBoxContent', null, ['string', 'string']);
    updateTextInCpp(currentTextboxId, updatedText);

    hideFormattingMenu();
    console.log("[saveAndExit] Text saved and menu hidden for:", currentTextboxId);
    currentTextboxId = null;
  }

  // Handle image resizing
  const observer = new ResizeObserver(entries => {
    for (let entry of entries) {
      const el = entry.target;
      const widthVW = (parseFloat(el.style.width) || el.offsetWidth) / window.innerWidth * 100;
      const heightVH = (parseFloat(el.style.height) || el.offsetHeight) / window.innerHeight * 100;
  
      if (!isNaN(widthVW) && !isNaN(heightVH)) {
        const updateImageSizeInCpp = Module.cwrap("call_updateImageSize", null, ["string", "number", "number"]);
        updateImageSizeInCpp(el.id, Math.round(widthVW), Math.round(heightVH));
      }
    }
  });
  
  document.querySelectorAll('.image').forEach(el => observer.observe(el));
  

  // Export functions
  window.showFormattingMenu = showFormattingMenu;
  window.hideFormattingMenu = hideFormattingMenu;
  window.applyFormatting = applyFormatting;
  window.saveAndExit = saveAndExit;
});
