// Debug script to check available methods on RichTextState
window.addEventListener("DOMContentLoaded", () => {
  console.log("Debug script loaded");
  
  Module().then((instance) => {
    const app = new instance.RichTextState();
    
    console.log("Available methods on RichTextState instance:");
    console.log("app.has_format =", app.has_format);
    
    try {
      const hasBold = app.has_format("bold");
      console.log("has_format('bold') result:", hasBold);
    } catch (e) {
      console.error("Error calling has_format:", e);
    }
    
    console.log("All properties of app:");
    for (const prop in app) {
      console.log(`- ${prop}: ${typeof app[prop]}`);
    }
  }).catch(err => {
    console.error("Module loading error:", err);
  });
}); 