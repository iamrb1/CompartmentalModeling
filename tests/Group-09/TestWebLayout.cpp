/**
 * @file TestWebLayout.cpp]
 *
 * @author Mary Holt
 *
 * Compile with: emcc -std=c++20 TestWebLayout.cpp ../../Group-09/Image/Image.cpp ../../Group-09/WebLayout/WebLayout.cpp -o TestWebLayout.html  --shell-file testingIndex.html
 * Then run: python -m http.server
 * Navigate to: http://localhost:8000/TestWebLayout.html
 * Check console for error messages and check elements for created test layouts
 */

#include <iostream>

#include "../../Group-09/WebLayout/WebLayout.hpp"

using namespace cse;

void testCase1() {
  // Default constructor test
  std::cout << "Beginning Test Case 1 (Default)\n";

  WebLayout wb;

  if (wb.getImages().size() != 0) { std::cout << "Image Innit Fail\n"; }
  if (wb.getTextBoxes().size() != 0) { std::cout << "Text Boxes Init Fail\n"; }

  std::cout << "Ending Test Case 1\n";
}

void testCase2() {
  // Test Adding
  std::cout << "Beginning Test Case 2 (Add)\n";

  WebLayout wb;
  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);

  //Recheck size of images vector
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }

  //Check saved correctly
  if (wb.getImages().at(0).image->getURL() != "https://msu_logo.png") { std::cout << "Image adding failure\n"; }
  if (wb.getImages().at(0).image->getWidth() != 50) { std::cout << "Image adding failure\n"; }
  if (wb.getImages().at(0).image->getHeight() != 50) { std::cout << "Image adding failure\n"; }
  if (wb.getImages().at(0).xPos != 10) { std::cout << "Image adding failure\n"; }
  if (wb.getImages().at(0).yPos != 10) { std::cout << "Image adding failure\n"; }

  FormattedText ft;
  ft.setText("CSE498");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  //Recheck size of text boxes vector
  if (wb.getTextBoxes().size() != 1) { std::cout << "Text box adding failure\n"; }

  //Check saved correctly
  if (wb.getTextBoxes().at(0).textBox->getFormattedText().getText() != "CSE498") {
    std::cout << "Text box adding failure\n";
  }
  if (wb.getTextBoxes().at(0).textBox->getWidth() != 100) { std::cout << "Text box adding failure\n"; }
  if (wb.getTextBoxes().at(0).textBox->getHeight() != 100) { std::cout << "Text box adding failure\n"; }
  if (wb.getTextBoxes().at(0).xPos != 12) { std::cout << "Text box adding failure\n"; }
  if (wb.getTextBoxes().at(0).yPos != 12) { std::cout << "Text box adding failure\n"; }

  std::cout << "Ending Test Case 2\n";
}

void testCase3() {
  // Test Removing
  std::cout << "Beginning Test Case 3 (Valid Remove)\n";

  WebLayout wb;
  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);

  //Check size of images vector
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }

  wb.removeImage(il);
  //Check size of images vector
  if (wb.getImages().size() != 0) { std::cout << "Image removing failure\n"; }

  FormattedText ft;
  ft.setText("CSE498");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  //Recheck size of text boxes vector
  if (wb.getTextBoxes().size() != 1) { std::cout << "Text box adding failure\n"; }

  wb.removeTextBox(tbl);

  //Recheck size of text boxes vector
  if (wb.getTextBoxes().size() != 0) { std::cout << "Text box removing failure\n"; }

  std::cout << "Ending Test Case 3\n";
}

void testCase4() {
  // Invalid Remove
  std::cout << "Beginning Test Case 4 (Invalid Remove)\n";

  WebLayout wb;
  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);

  //Check size of images vector
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }

  std::shared_ptr<Image> ii = std::make_shared<Image>("https://msu_logo_but_bigger.png", 150, 150);
  ImageLayout iil(ii, 10, 10);
  wb.removeImage(iil);
  //Check size of images vector
  if (wb.getImages().size() != 1) { std::cout << "Image Invalid Removal failure\n"; }

  FormattedText ft;
  ft.setText("CSE498");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  //Recheck size of text boxes vector
  if (wb.getTextBoxes().size() != 1) { std::cout << "Text box adding failure\n"; }

  FormattedText ft2;
  ft.setText("CSE498-Capstone");
  TextBoxConfig tbc2;
  tbc2.width = 200;
  tbc2.height = 200;
  tbc2.content = ft2;
  std::shared_ptr<TextBox> tbb = std::make_shared<TextBox>("", tbc2);
  TextBoxLayout tbl2(tbb, 14, 14);
  wb.removeTextBox(tbl2);
  //Check size of text boxes vector
  if (wb.getTextBoxes().size() != 1) { std::cout << "Text box Invalid Removal failure\n"; }

  std::cout << "Ending Test Case 4\n";
}

void testCase6() {
  // Edge Cases
  std::cout << "Beginning Test Case 6 (Edge Cases)\n";

  WebLayout wb;
  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);

  //Check size of images vector
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }
  wb.addImage(il);
  if (wb.getImages().size() != 2) { std::cout << "Image adding failure\n"; }

  // Delete and check size again (should only delete one instance of image)
  wb.removeImage(il);
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }

  std::cout << "Ending Test Case 6 (Edge Cases)\n";
}

void testCase7() {
  // Testing WebLayout id
  std::cout << "Beginning Test Case 7 (ID)\n";

  WebLayout wb;

  std::string idBeforeAdd = wb.getID();

  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);

  //Check size of images vector
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }
  // Make sure ID didn't get changed
  if (wb.getID() != idBeforeAdd) { std::cout << "Image adding corrupted ID\n"; }

  WebLayout wb2;
  if (wb.getID() == wb2.getID()) { std::cout << "ID incrementation error!\n"; }

  std::cout << "Ending Test Case 7 (ID)\n";
}

void testCase8() {
  // Testing Get Items from IDs
  std::cout << "Beginning Test Case 8 (Get Item from ID)\n";

  WebLayout wb;

  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);
  //Check size of images vector (ensure added correctly)
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }
  std::string testID = il.image->getID();
  auto retrievedImage = wb.getImageFromID(testID);
  if(retrievedImage.image != il.image) { std::cout << "Get Image from ID Error!\n"; }

  FormattedText ft;
  ft.setText("CSE498");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  //Check size of textbox vector (ensure added correctly)
  if (wb.getTextBoxes().size() != 1) { std::cout << "Textbox adding failure\n"; }
  testID = tbl.textBox->getID();
  auto retrievedTextBox = wb.getTextboxFromID(testID);
  if(retrievedTextBox.textBox != tbl.textBox) { std::cout << "Get Image from ID Error!\n"; }

  std::cout << "Ending Test Case 8 (Get Item from ID)\n";
}

void testCase9() {
  // Testing modifying of image (setPosition, setSize, contains)
  std::cout << "Beginning Test Case 9 (Modifying Image)\n";

  WebLayout wb;

  std::shared_ptr<Image> i = std::make_shared<Image>("https://msu_logo.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);
  //Check size of images vector (ensure added correctly)
  if (wb.getImages().size() != 1) { std::cout << "Image adding failure\n"; }
  std::string testID = il.image->getID();

  // Change position of image
  wb.setPosition(testID, 50, 50);
  auto retrievedImage = wb.getImageFromID(testID);
  if(retrievedImage.xPos != 50 || retrievedImage.yPos != 50) { std::cout << "Change position of Image Error!\n"; }

  // Change size
  wb.setSize(testID, 100, 100);
  retrievedImage = wb.getImageFromID(testID);
  if(retrievedImage.image->getHeight() != 100 || retrievedImage.image->getWidth() != 100) { std::cout << "Change size of Image Error!\n"; }

  // Check if contains
  if(!wb.contains(testID)) { std::cout << "Checking layout contains image error!\n"; }
  // Check wrong contains works
  std::shared_ptr<Image> i2 = std::make_shared<Image>("https://msu_logo.png", 5, 5);
  ImageLayout il2(i2, 40, 40);
  std::string wrongID = il2.image->getID();
  if(wb.contains(wrongID)) { std::cout << "Web Layout contains error! (Wrongfully detected image)\n"; }

  std::cout << "Ending Test Case 9 (Modifying Image)\n";

}

void testCase10() {
  // Testing modifying of textbox (setPosition, setSize, contains)
  std::cout << "Beginning Test Case 10 (Modifying TextBox)\n";

  WebLayout wb;

  FormattedText ft;
  ft.setText("CSE498");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  std::string testID = tbl.textBox->getID();

  // Change position of image
  wb.setPosition(testID, 50, 50);
  auto retrievedTextBox = wb.getTextboxFromID(testID);
  if(retrievedTextBox.xPos != 50 || retrievedTextBox.yPos != 50) { std::cout << "Change position of TextBox Error!\n"; }

  // Change size
  wb.setSize(testID, 50, 50);
  retrievedTextBox = wb.getTextboxFromID(testID);
  if(retrievedTextBox.textBox->getHeight() != 50 || retrievedTextBox.textBox->getWidth() != 50) { std::cout << "Change size of TextBox Error!\n"; }

  // Check if contains
  if(!wb.contains(testID)) { std::cout << "Checking layout contains textbox error!\n"; }
  // Check wrong contains works
  FormattedText ft2;
  ft2.setText("CSE498 RULES");
  TextBoxConfig tbc2;
  tbc2.width = 10;
  tbc2.height = 10;
  tbc2.content = ft2;
  std::shared_ptr<TextBox> tb2 = std::make_shared<TextBox>("", tbc2);
  TextBoxLayout tbl2(tb2, 20, 20);

  std::string wrongID = tbl2.textBox->getID();
  if(wb.contains(wrongID)) { std::cout << "Web Layout contains error! (Wrongfully detected textbox)\n"; }

  std::cout << "Ending Test Case 10 (Modifying TextBox)\n";

}

void testCase11() {
  // Testing textBoxLayout + imageLayout
  std::cout << "Beginning Test Case 11 (textBoxLayout + imageLayout)\n";

  FormattedText ft;
  ft.setText("YOU SHOULDN'T SEE THIS TEXT");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  TextBoxLayout tblCopy = tbl;

  std::shared_ptr<Image> i = std::make_shared<Image>("https://upload.wikimedia.org/wikipedia/en/thumb/a/a7/Michigan_State_Athletics_logo.svg/1200px-Michigan_State_Athletics_logo.svg.png", 50, 50);
  ImageLayout il(i, 10, 10);
  ImageLayout ilCopy = il;

  // Test == operator works
  if(tblCopy != tbl) { std::cout << "Layout == operator Error!\n"; }
  if(ilCopy != il) { std::cout << "Layout == operator Error!\n"; }

  FormattedText ft2;
  ft2.setText("THIS IS DIFFERENT TEXT");
  TextBoxConfig tbc2;
  tbc2.width = 10;
  tbc2.height = 50;
  tbc2.content = ft2;
  std::shared_ptr<TextBox> tb2 = std::make_shared<TextBox>("", tbc2);
  TextBoxLayout tbl2(tb2, 12, 12);

  std::shared_ptr<Image> i2 = std::make_shared<Image>("https://upload.wikimedia.org/wikipedia/en/thumb/a/a7/Michigan_State_Athletics_logo.svg/1200px-Michigan_State_Athletics_logo.svg.png", 10, 30);
  ImageLayout il2(i2, 70, 15);

  // Test != operator
  if(tbl == tbl2) { std::cout << "Layout == operator can't detect different ids\n"; }
  if(il == il2) { std::cout << "Layout == operator can't detect different ids\n"; }

  std::cout << "Ending Test Case 11 (textBoxLayout + imageLayout)\n";
}


void testCase12() {
  // Testing toggling text boxes and items (fully UI testing)
  std::cout << "Beginning Test Case 12 (Toggling Items)\n";

  WebLayout wb;

  FormattedText ft;
  ft.setText("CSE498");
  TextBoxConfig tbc;
  tbc.width = 45;
  tbc.height = 45;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  FormattedText ft2;
  ft2.setText("CSE498-Capstone");
  TextBoxConfig tbc2;
  tbc2.width = 20;
  tbc2.height = 20;
  tbc2.content = ft2;
  std::shared_ptr<TextBox> tbb = std::make_shared<TextBox>("", tbc2);
  TextBoxLayout tbl2(tbb, 30, 30);
  wb.addTextBox(tbl2);

  std::shared_ptr<Image> i = std::make_shared<Image>("https://upload.wikimedia.org/wikipedia/en/thumb/a/a7/Michigan_State_Athletics_logo.svg/1200px-Michigan_State_Athletics_logo.svg.png", 50, 50);
  ImageLayout il(i, 10, 10);
  wb.addImage(il);

  std::shared_ptr<Image> i2 = std::make_shared<Image>("https://upload.wikimedia.org/wikipedia/en/thumb/a/a7/Michigan_State_Athletics_logo.svg/1200px-Michigan_State_Athletics_logo.svg.png", 10, 30);
  ImageLayout il2(i2, 70, 15);
  wb.addImage(il2);

  // Activate whole layout
  wb.activateLayout();

  // Toggle specific item (should turn off)
  wb.toggleTextBox(tbl);
  wb.toggleImage(il);

  // Expected behavior on UI side is second textbox/image still visible, while first is not

  std::cout << "Ending Test Case 12 (Toggling Items)\n";
}

void testCase13() {
  // Testing toggling layouts (fully UI testing)
  std::cout << "Beginning Test Case 13 (Toggling Layout)\n";

  WebLayout wb;

  FormattedText ft;
  ft.setText("YOU SHOULDN'T SEE THIS TEXT");
  TextBoxConfig tbc;
  tbc.width = 100;
  tbc.height = 100;
  tbc.content = ft;
  std::shared_ptr<TextBox> tb = std::make_shared<TextBox>("", tbc);
  TextBoxLayout tbl(tb, 12, 12);
  wb.addTextBox(tbl);

  // Activate whole layout
  wb.activateLayout();

  // Deactivate layout (should not show on UI)
  wb.deactivateLayout();

  std::cout << "Ending Test Case 13 (Toggling Layout)\n";
}

int main() {

  testCase1();
  testCase2();
  testCase3();
  testCase4();
  // testCase5(); -- Was removed load page function --
  testCase6();
  testCase7();
  testCase8();
  testCase9();
  testCase10();
  testCase11();
  testCase12();
  testCase13();  //Do this case last because of UI

  return 0;
}
