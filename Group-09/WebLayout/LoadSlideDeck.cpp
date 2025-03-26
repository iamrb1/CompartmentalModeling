// LoadSlideDeck.cpp
#include <emscripten.h>

#include <nlohmann/json.hpp>

#include "../Image/image.hpp"
#include "../TextBox/TextBox.hpp"
#include "WebLayout.hpp"
#include "WebLayoutManager.hpp"

// Allows for easy JSON parsing, building JSON objects/arrays, pretty printing,
// etc.
using json = nlohmann::json;
using namespace cse;

// Shared global layout manager
WebLayoutManager* g_manager = nullptr;

extern "C" {

EMSCRIPTEN_KEEPALIVE
void loadSlideDeckFromJson(const char* jsonStr) {
  if (!g_manager) g_manager = new WebLayoutManager();

  json deck = json::parse(jsonStr);

  for (const auto& slide : deck["slides"]) {
    auto layout = std::make_shared<WebLayout>();

    // Load all text boxes
    if (slide.contains("textBoxes")) {
      for (const auto& tbJson : slide["textBoxes"]) {
        FormattedTextConfig ftConfig{tbJson["text"], TextType::Raw,
                                     tbJson["font"], tbJson["fontSize"],
                                     tbJson["color"]};
        TextBoxConfig tbConfig{FormattedText(ftConfig), tbJson["width"],
                               tbJson["height"]};
        auto textbox = std::make_shared<TextBox>("", tbConfig);
        layout->addTextBox(TextBoxLayout(textbox, tbJson["x"], tbJson["y"]));
      }
    }

    // Load all images
    if (slide.contains("images")) {
      for (const auto& imgJson : slide["images"]) {
        auto image = std::make_shared<Image>(imgJson["url"], imgJson["width"],
                                             imgJson["height"]);
        layout->addImage(ImageLayout(image, imgJson["x"], imgJson["y"]));
      }
    }

    layout->loadPage();            // Render the layout
    g_manager->addLayout(layout);  // Add to the global manager
  }

  g_manager->initialize();  // Show the first slide
}

}  // extern "C"
