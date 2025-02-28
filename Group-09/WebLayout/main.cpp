/**
 * @file main.cpp
 * @author merry
 */

#include <iostream>
#include <memory>
#include "WebLayout.h"

// Compile with: emcc main.cpp ../Image/Image.cpp WebLayout.cpp -o output.js --shell-file index.html
using namespace cse;

int main() {
  // Create WebLayout
  std::shared_ptr<WebLayout> sampleWebLayout = std::make_shared<WebLayout>();

  // Setup test Image
  std::shared_ptr<Image> testImage = std::make_shared<Image>(
      "https://media.discordapp.net/attachments/1328474944107184178/1343979939451506790/IMG_8493.jpg?ex=67bfe71b&is=67be959b&hm=d96ae7db5c8441beae947db231f2abe788c2e0bde2b93f1334bacd4dabdda3e0&=&format=webp&width=454&height=671",
      35,
      65);
  ImageLayout il(testImage, 10, 10);
  sampleWebLayout->addImage(il);

  // Setup test textbox
  TextBoxConfig tbc;
  tbc.height = 10;
  tbc.width = 45;
  tbc.text = "Yay a Text Box!";
  std::shared_ptr<TextBox> testTextBox = std::make_shared<TextBox>(tbc);
  TextBoxLayout tbl(testTextBox, 40, 30);
  sampleWebLayout->addTextBox(tbl);

  // Setup test textbox2 (exceed barriers test)
  TextBoxConfig tbc2;
  tbc2.height = 15;
  tbc2.width = 20;
  tbc2.text = "I'm an out of bounds text box D:";
  std::shared_ptr<TextBox> testTextBox2 = std::make_shared<TextBox>(tbc2);
  TextBoxLayout tbl2(testTextBox2, 500, 450);
  sampleWebLayout->addTextBox(tbl2);

  // Load Page
  sampleWebLayout->loadPage();
}