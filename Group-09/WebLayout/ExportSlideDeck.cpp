// ExportSlideDeck.cpp
#include <emscripten.h>

#include "../Image/image.hpp"
#include "../TextBox/TextBox.hpp"
#include "../utils/nlohmann/json.hpp"
#include "WebLayout.hpp"
#include "WebLayoutManager.hpp"

// Allows for easy JSON parsing, building JSON objects/arrays, pretty printing,
// etc.
using json = nlohmann::json;
using namespace cse;

extern WebLayoutManager* g_manager;

extern "C" {

EMSCRIPTEN_KEEPALIVE
const char* exportSlideDeckToJson() {
  static std::string exportedJson;
  json deck;
  deck["slides"] = json::array();

  if (!g_manager) return "{}";

  for (const auto& layout : g_manager->getLayouts()) {
    json slide;

    slide["textBoxes"] = json::array();
    for (const auto& tb : layout->getTextBoxes()) {
      const auto& text = tb.textBox->getFormattedText();
      slide["textBoxes"].push_back({{"text", text.getText()},
                                    {"font", text.getFont()},
                                    {"fontSize", text.getFontSize()},
                                    {"color", text.getColor()},
                                    {"width", tb.textBox->getWidth()},
                                    {"height", tb.textBox->getHeight()},
                                    {"x", tb.xPos},
                                    {"y", tb.yPos}});
    }

    slide["images"] = json::array();
    for (const auto& img : layout->getImages()) {
      slide["images"].push_back({{"url", img.image->getURL()},
                                 {"width", img.image->getWidth()},
                                 {"height", img.image->getHeight()},
                                 {"x", img.xPos},
                                 {"y", img.yPos}});
    }

    deck["slides"].push_back(slide);
  }

  exportedJson = deck.dump(2);  // pretty print
  return exportedJson.c_str();
}

}  // extern "C"
