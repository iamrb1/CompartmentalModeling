/**
* @file TestApplication.cpp
 *
 * @author Grace Fitzgerald
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "MockClasses/MockPresentationManager.hpp"
#include "MockClasses/MockPresentationEventManager.hpp"
#include "MockClasses/MockWebLayout.cpp"
#include "MockClasses/MockImage.cpp"


using namespace cse;

TEST_CASE("Initial state is correct","[PresentationManager]") {
 MockPresentationManager mockManager;
 REQUIRE(mockManager.getNumSlides() == 0);
 REQUIRE(mockManager.getCurrentPos() == 0);
 REQUIRE(mockManager.isRunning() == false);

}

TEST_CASE("Add one slide","[PresentationManager]") {
 MockPresentationManager mockManager;
 REQUIRE(mockManager.getNumSlides() == 0);
 REQUIRE(mockManager.getCurrentPos() == 0);
 REQUIRE(mockManager.isRunning() == false);
 mockManager.addNewSlide();
 REQUIRE(mockManager.getNumSlides() == 1);
 REQUIRE(mockManager.getCurrentPos() == 0);
 REQUIRE(mockManager.isRunning() == false);

}

TEST_CASE("Add multiple","[PresentationManager]") {
 MockPresentationManager mockManager;
 REQUIRE(mockManager.getNumSlides() == 0);
 REQUIRE(mockManager.getCurrentPos() == 0);
 REQUIRE(mockManager.isRunning() == false);
 for(int i = 0; i < 10; i++) {
  mockManager.addNewSlide();
  REQUIRE(mockManager.getNumSlides() == i+1);
  REQUIRE(mockManager.getCurrentPos() == i);
  REQUIRE(mockManager.isRunning() == false);
 }


}

TEST_CASE("Clear slides","[PresentationManager]") {
 MockPresentationManager mockManager;
 for(int i = 0; i < 10; i++) {
  mockManager.addNewSlide();
 }
 REQUIRE(mockManager.getNumSlides() == 10);
 REQUIRE(mockManager.getCurrentPos() == 9);
 REQUIRE(mockManager.isRunning() == false);

 mockManager.clear();
 REQUIRE(mockManager.getNumSlides() == 0);
 REQUIRE(mockManager.getCurrentPos() == 0);

}

TEST_CASE("Start and Stop Events","[PresentationManager]") {
 MockPresentationManager mockManager;
 for(int i = 0; i < 10; i++) {
  mockManager.addNewSlide();
 }
 REQUIRE(mockManager.isRunning() == false);
 mockManager.start();
 REQUIRE(mockManager.isRunning() == true);
}

TEST_CASE("Pushing and deleting a slide","[PresentationManager]") {
 MockPresentationManager mockManager;
 auto slide = std::make_shared<MockWebLayout>();
 auto slide2 = std::make_shared<MockWebLayout>();
 mockManager.pushBackSlide(slide);
 mockManager.pushBackSlide(slide2);
 REQUIRE(mockManager.getNumSlides() == 2);
 REQUIRE(mockManager.getCurrentPos() == 0);
 mockManager.deleteSlide(slide);
 REQUIRE(mockManager.getNumSlides() == 1);
 REQUIRE(mockManager.getCurrentPos() == 0);
}

TEST_CASE("Change slides","[PresentationManager]") {
 MockPresentationManager mockManager;
 for(int i = 0; i < 10; i++) {
  mockManager.addNewSlide();
 }
 mockManager.goTo(9); //Valid
 REQUIRE(mockManager.getCurrentPos() == 9);
 mockManager.goTo(11); //Invalid
 REQUIRE(mockManager.getCurrentPos() == 9);

}

TEST_CASE("Advance slides","[PresentationManager]") {
 MockPresentationManager mockManager;
 for(int i = 0; i < 10; i++) {
  mockManager.addNewSlide();
 }
 REQUIRE(mockManager.getNumSlides() == 10);
 mockManager.goTo(0);
 for(int i = 0; i < 10; i++) {
  REQUIRE(mockManager.getCurrentPos() == i);
  mockManager.advance();
 }
 mockManager.advance(); //Attempt to go past end of slides
 REQUIRE(mockManager.getCurrentPos() == 9);
}

TEST_CASE("Adding a textbox","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 mockManager.addTextBox();
 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(mockManager.getCurrentPos() == 0);
 REQUIRE(slide->getTextBoxes().size() == 1);
}

TEST_CASE("Adding an image","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 std::string url = "https://encrypted-tbn0.gstatic.com/images";
 mockManager.addImage(url, 100, 100, "image" );
 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(slide->getImages().size() == 1);
}

TEST_CASE("Adding an image with invalid dimensions","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 std::string url = "https://encrypted-tbn0.gstatic.com/images";
 mockManager.addImage(url, -100, 100, "image" );
 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(slide->getImages().size() == 0);

 mockManager.addImage(url, 100, -100, "image" );
 slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(slide->getImages().size() == 0);

 mockManager.addImage(url, -100, -100, "image" );
 slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(slide->getImages().size() == 0);
}

TEST_CASE("Update object position and size","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 mockManager.addTextBox();
 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(!slide->getTextBoxes().empty());
 auto id = slide->getTextBoxes().at(0).textBox->getID();
 mockManager.updatePosition(id, 30, 50);
 mockManager.updateSize(id, 150, 200);

 slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 auto layout = slide->getTextboxFromID(id); // textbox layout

 REQUIRE(layout.xPos == 30);
 REQUIRE(layout.yPos == 50);
 REQUIRE(layout.textBox->getWidth() == 150);
 REQUIRE(layout.textBox->getHeight() == 200);
}

TEST_CASE("Moveable object check returns true for textbox","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 mockManager.addTextBox();
 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 auto id = slide->getTextBoxes().at(0).textBox->getID();
 REQUIRE(mockManager.isMoveableObject(id));
}

TEST_CASE("Moveable object check returns true for image","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 std::string url = "https://encrypted-tbn0.gstatic.com/images";
 mockManager.addImage(url, 100, 100, "image" ); // id = "image-1"
 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 REQUIRE(slide->getImages().size() == 1);
 auto id = slide->getImages().at(0).image->getID();
 REQUIRE(mockManager.isMoveableObject(id));
}

TEST_CASE("Moveable object check returns false for unknown id","[PresentationManager]") {
 MockPresentationManager mockManager;
 REQUIRE(mockManager.isMoveableObject("non-existing-id") == false);
}

TEST_CASE("Adding slide change event stores timing and index", "[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 mockManager.addNewSlide();
 mockManager.addSlideChangeEvent(10, 0, 1);

// auto transitions = mockManager.getTransitionEvents();
// REQUIRE(transitions.contains(0));
// REQUIRE(std::get<0>(transitions.at(0)) == 10);
// REQUIRE(std::get<1>(transitions.at(0)) == 1);
}

TEST_CASE("Adding object events for textbox and image","[PresentationManager]") {
 MockPresentationManager mockManager;
 mockManager.addNewSlide();
 mockManager.addTextBox();
 std::string url = "https://encrypted-tbn0.gstatic.com/images";
 mockManager.addImage(url, 100, 100, "image" ); // id = "image-1"

 auto slide = mockManager.getSlides().at(mockManager.getCurrentPos());
 auto idText = slide->getTextBoxes().at(0).textBox->getID();
 auto idImage = slide->getImages().at(0).image->getID();
 mockManager.addObjectEvent(7, 0, idText);
 mockManager.addObjectEvent(8, 0, idImage);


}

TEST_CASE("deleteSlide correctly removes events from the specified slide", "[PresentationEventManager]") {
 // Arrange
 MockPresentationManager mockManager;
 MockPresentationEventManager manager(&mockManager);

 manager.resize(2);

 auto dummyFn = [](MockPresentationManager*, size_t) {};

 manager.addEvent(dummyFn, 0, "textbox-1", 0);
 manager.addEvent(dummyFn, 1, "textbox-2", 0);

 manager.deleteSlide(0);

 // The event from slide 0 should be removed from _id_to_slide
 auto infoSlide0 = manager.getSlideEventInfo(0);  // Slide 0 is now slide 1
 REQUIRE(infoSlide0.find("textbox-1") == infoSlide0.end());

 // The event from what was originally slide 1 should still exist
 auto infoSlide1 = manager.getSlideEventInfo(0);
 REQUIRE(infoSlide1.find("textbox-2") != infoSlide1.end());
}