window.addEventListener("DOMContentLoaded", () => {
  // 1) Initialize our Emscripten module
  Module().then((instance) => {
    // 2) Grab references to the tab content areas
    const htmlContainer      = document.getElementById("tab-html");
    const mdCodeElement      = document.getElementById("md-code");
    const rtfCodeElement     = document.getElementById("rtf-code");
    const rawHTMLCodeElement = document.getElementById("raw-html-code");

    // 3) Call each bound function from RichTextBindings
    //    and set them into the correct tab container

    // HTML (this returns a *full* HTML document)
    const fullHTML = instance.renderRichTextHTML();
    // Insert it "rendered" into the HTML tab
    htmlContainer.innerHTML = fullHTML;

    // Markdown
    const markdownOutput = instance.renderRichTextMarkdown();
    mdCodeElement.textContent = markdownOutput;

    // RTF
    const rtfOutput = instance.renderRichTextRTF();
    rtfCodeElement.textContent = rtfOutput;

    // Raw HTML (the “bare” HTML string)
    const rawHTML = instance.renderRichTextRawHTML();
    rawHTMLCodeElement.textContent = rawHTML;

    // 4) Tab switching logic
    const tabButtons  = document.querySelectorAll("#tab-buttons button");
    const tabContents = document.querySelectorAll(".tab-content");

    // Hide them all initially
    tabContents.forEach(div => div.classList.remove("active"));
    // Show the first tab (HTML) by default
    document.getElementById("tab-html").classList.add("active");

    tabButtons.forEach(btn => {
      btn.addEventListener("click", () => {
        // Hide all
        tabContents.forEach(div => div.classList.remove("active"));
        // Show only the chosen tab
        const targetId = btn.getAttribute("data-tab-target");
        document.getElementById(targetId).classList.add("active");
      });
    });

    // 5) Render your left‐side input bar as before
    const bar = document.getElementById("toolbar");
    const wrappedInput = MakeInput.textInput("", "Type here...");
    wrappedInput.style.width = "100%";
    bar.appendChild(wrappedInput);
  });
});
