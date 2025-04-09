window.addEventListener("DOMContentLoaded", () => {
    Module().then((instance) => {
      const html = instance.renderRichTextHTML();
      document.querySelector("output").innerHTML = html;
  
      const bar = document.getElementById("toolbar");
  
      const wrappedInput = MakeInput.textInput("", "Type here...");
      wrappedInput.style.width = "100%";  
  
      bar.appendChild(wrappedInput);
    });
  });
  