/**
 * @file main.cpp
 * @author Mary Holt, Allie Ianello
 */

#include <unistd.h>

#include "WebLayout.hpp"
#include "WebLayoutManager.hpp"

// Compile from /Group-09/WebLayout with:
//  emcc main.cpp WebLayout.cpp WebLayoutManager.cpp LoadSlideDeck.cpp ExportSlideDeck.cpp ../TextBox/FormattedText.cpp ../Image/Image.cpp -I../utils -o output.js --shell-file index.html -s WASM=1 -s EXPORTED_FUNCTIONS="['_main', '_call_advance', '_call_rewind', '_loadSlideDeckFromJson', '_exportSlideDeckToJson', '_call_addNewSlide']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']"

// Bring in the global layout manager used by load/save functions
extern cse::WebLayoutManager *g_manager;

using namespace cse;

int main() {
  // Initialize the global layout manager
  g_manager = new WebLayoutManager();

  // // Create First WebLayout
   std::shared_ptr<WebLayout> sampleWebLayout = std::make_shared<WebLayout>();

  // // Setup test Image
//   std::shared_ptr<Image> testImage = std::make_shared<Image>(
//       "https://encrypted-tbn0.gstatic.com/"
//       "images?q=tbn:ANd9GcREOGiXHLq71nPqfRAEKdCMou6nlTbQPf-HJQ&s",
//       35, 65);
//   ImageLayout il(testImage, 10, 10);
  // sampleWebLayout->addImage(il);

  // // Setup test textbox

//   FormattedText ft;
//   ft.setText("Yay a Text Box!");
//   TextBoxConfig tbc;
//   tbc.content = ft;
//   tbc.height = 10;
//   tbc.width = 45;
//   std::shared_ptr<TextBox> testTextBox = std::make_shared<TextBox>("", tbc);

//   TextBoxLayout tbl(testTextBox, 40, 30);
//   sampleWebLayout->addTextBox(tbl);

  // // Setup test textbox2 (exceed barriers test)
//   FormattedText ft2;
//   ft2.setText("I'm an out of bounds text box D:");
//   TextBoxConfig tbc2;
//   tbc2.content = ft2;
//   tbc2.height = 15;
//   tbc2.width = 20;
//   std::shared_ptr<TextBox> testTextBox2 = std::make_shared<TextBox>("",
//   tbc2); TextBoxLayout tbl2(testTextBox2, 500, 450);
//   sampleWebLayout->addTextBox(tbl2);

  // // Load Page
//   sampleWebLayout->loadPage();
//
//   std::shared_ptr<WebLayout> sampleWebLayout2 =
//   std::make_shared<WebLayout>();
//   // Setup test Image
//   std::shared_ptr<Image> testImage2 = std::make_shared<Image>(
//       "https://encrypted-tbn0.gstatic.com/"
//       "images?q=tbn:ANd9GcREOGiXHLq71nPqfRAEKdCMou6nlTbQPf-HJQ&s",
//       15, 45);
//   ImageLayout il2(testImage2, 50, 60);
//   sampleWebLayout2->addImage(il2);

  // // Setup test textbox
//   FormattedText ft3;
//   ft3.setText("Third TextBox!");
//   TextBoxConfig tbc3;
//   tbc3.content = ft3;
//   tbc3.height = 30;
//   tbc3.width = 30;
//   std::shared_ptr<TextBox> testTextBox3 = std::make_shared<TextBox>("",
//   tbc3);

//   TextBoxLayout tbl3(testTextBox3, 10, 10);
//   sampleWebLayout2->addTextBox(tbl3);
//   sampleWebLayout2->loadPage();
//
//    WebLayoutManager manager;
//    manager.addLayout(sampleWebLayout);
//    manager.addLayout(sampleWebLayout2);
//    manager.initialize();

   // Add to the global manager
//   g_manager->addLayout(sampleWebLayout);
//   g_manager->addLayout(sampleWebLayout2);
//   g_manager->initialize();

  emscripten_exit_with_live_runtime();
}