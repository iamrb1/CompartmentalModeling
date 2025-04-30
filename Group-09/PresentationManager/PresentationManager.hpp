/**
 * @file PresentationManager.h
 * @brief This file contains the PresentationManager and the exported functions
 * called by JS
 * @author Owen Haiar, Mary Holt, Allie Ianello
 */

#pragma once

#include <nlohmann/json.hpp>

#include "../WebLayout/WebLayout.hpp"
#include "../js/EventListeners.hpp"
#include "PresentationEventManager.hpp"

using namespace cse;
using json = nlohmann::json;
using Slide = std::shared_ptr<WebLayout>;

/**
 * Main manager for Liskov presentation program
 * Initialized globally, and update() is called in a loop while running
 */
class PresentationManager {
 private:
  PresentationEventManager _event_manager;
  bool _running = false;
  int _current_pos = 0;
  std::vector<Slide> _slide_deck;

 public:
  /**
   * Construct, set the pointer for the eventmanager, bind js buttons
   */
  PresentationManager() : _event_manager(this) { bind(); }

  /**
   * Set the initial size of the deck
   */
  void initialize(const size_t size) {
    _slide_deck.reserve(size);
    _event_manager.resize(size, true);
  }

  /**
   * Clear the slides and event manager
   */
  void clear() {
    for (auto& layout : _slide_deck) {
      layout->deactivateLayout();  // Deactivate all layouts
    }
    _slide_deck.clear();
    _event_manager.clear();
    _current_pos = 0;
    _event_manager.onSlideChanged(_current_pos);
    onSlideChangedJS();
  }

  /**
   * Start the events
   */
  void start() {
    _event_manager.start();
    _running = true;
    std::cout << "Starting Events" << std::endl;
    // Toggle bottom bar
    toggleBottomNav(true);
    toggleBorders(true);
    EM_ASM({ document.body.classList.add("present-mode"); });
  }

  /**
   * Stop the events
   */
  void stop() {
    _event_manager.stop();
    _running = false;
    std::cout << "Stopping Events" << std::endl;
    EM_ASM({ document.body.classList.remove("present-mode"); });
  }

  /**
   * Main driver for timing
   */
  void update() { _event_manager.update(); }

  /**
   * Add a new slide to the deck, and go to it
   */
  void addNewSlide() {
    auto new_slide = std::make_shared<WebLayout>();
    _slide_deck.push_back(new_slide);
    _event_manager.resize(_slide_deck.size());  // Update the event manager
    goTo(_slide_deck.size() - 1);               // Go to the new slide
    std::cout << "Added new slide. ID: " << new_slide->getID() << std::endl;
    onSlideChangedJS();
  }

  /**
   * Removes a slide
   * @param slide
   */
  void deleteSlide(const Slide& slide) {
    const auto it = std::ranges::find(_slide_deck, slide);
    if (it == _slide_deck.end()) {
      return;  // Invalid slide
    }
    int slide_num = it - _slide_deck.begin();

    auto new_pos = _current_pos;
    if (slide_num <=
        _current_pos) {  // If slide is current or previous slide move back one
      new_pos = _current_pos != 0 ? _current_pos - 1 : _current_pos;
    }

    auto deck = json::parse(exportSlideDeckToJson(this));
    deck["slides"].erase(deck["slides"].begin() + slide_num);
    loadSlideDeckFromJson(deck.dump(2).c_str(), this);
    goTo(new_pos);

    /*
    slide->deactivateLayout();

    // Find index of slide
    const auto it = std::ranges::find(_slide_deck, slide);
    std::cout << "POSITION: " << it - _slide_deck.begin() << std::endl;

    std::erase(_slide_deck, slide);
    std::cout << "Deleted slide. ID: " << slide->getID() << std::endl;

    if(_current_pos != 0) {
            _current_pos--;
    }
    _event_manager.onSlideChanged(_current_pos);
    onSlideChangedJS();
    */
  }

  /**
   * Add a given slide to end of the deck
   */
  void pushBackSlide(const Slide& slide) {
    _slide_deck.push_back(slide);
    _event_manager.resize(_slide_deck.size());
    std::cout << "Pushed slide: " << slide->getID() << std::endl;
    onSlideChangedJS();
  }

  /**
   * Advances to the next slide
   */
  void advance() { goTo(_current_pos + 1); }

  /**
   * Rewinds to the previous slide
   */
  void rewind() { goTo(_current_pos - 1); }

  /**
   * Advances to the next event, or next slide if no events remain
   */
  void next() {
    if (_event_manager.next()) return;
    advance();
  }

  /**
   * Add a new text box to the current slide
   */
  void addTextBox() {
    // Create a new text box with predefined properties
    FormattedText ft;
    ft.setText("New Box!");
    TextBoxConfig tbc;
    tbc.content = ft;
    tbc.height = 10;
    tbc.width = 30;
    auto newTextBox = std::make_shared<TextBox>("", tbc);
    TextBoxLayout newLayout = {newTextBox, 50, 50};

    if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) {
      std::cout << "ERROR: No current layout to modify.\n";
      // Create new slide and finish adding textbox
      addNewSlide();
    }

    _slide_deck.at(_current_pos)->addTextBox(newLayout);
    _slide_deck.at(_current_pos)->activateLayout();
  }

  /**
   * Add a new image to the current slide
   */
  void addImage(const std::string& url, int width, int height,
                const std::string& altText) {
    if (width <= 0 || height <= 0) {
      std::cout << "ERROR: Invalid image dimensions: width=" << width
                << ", height=" << height << std::endl;
      return;
    }

    if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) {
      std::cout << "ERROR: No current layout to add image.\n";
      // Create new slide and finish adding image
      addNewSlide();
    }

    auto image = std::make_shared<Image>(url, width, height, altText);
    ImageLayout layout{image, 10, 10};
    _slide_deck.at(_current_pos)->addImage(layout);
    _slide_deck.at(_current_pos)->activateLayout();
  }

  /**
   * Go to a given slide
   * @param slide_num
   */
  void goTo(const int slide_num) {
    std::cout << "Going to slide " << (slide_num + 1) << std::endl;

    // If running presentation and last slide, go to end
    if (_running && slide_num >= _slide_deck.size()) {
      std::cout << "END OF PRESENTATION" << std::endl;
      auto currentLayout = _slide_deck.at(_current_pos);
      currentLayout->deactivateLayout();
      toggleEndScreen(true);
    }

    if (slide_num < 0 || slide_num >= _slide_deck.size()) {
      std::cout << "ERROR: Invalid slide number: " << (slide_num + 1)
                << std::endl;
      return;  // Exit early
    }

    auto currentLayout = _slide_deck.at(_current_pos);
    if (currentLayout) {
      currentLayout->deactivateLayout();
    }

    _current_pos = slide_num;

    currentLayout = _slide_deck.at(_current_pos);
    currentLayout->activateLayout();
    _event_manager.onSlideChanged(_current_pos);
    onSlideChangedJS();
  }

  void toggleEndScreen(bool visible) {
    std::string visibility = "hidden";

    if (visible) {
      visibility = "visible";
    }

    EM_ASM(
        {
          var vis = UTF8ToString($0);
          var layoutDiv = document.getElementById('end-slide');
          if (layoutDiv) {
            layoutDiv.style.visibility = vis;
          }
        },
        visibility.c_str());
  }

  /**
   * Updates object position
   */
  void updatePosition(std::string id, int newX, int newY) {
    if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) return;
    _slide_deck.at(_current_pos)->setPosition(id, newX, newY);
  }

  /**
   * Updates object size
   */
  void updateSize(std::string id, int newWidth, int newHeight) {
    if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) return;
    _slide_deck.at(_current_pos)->setSize(id, newWidth, newHeight);
  }

  /**
   * Returns if id is a moveable item
   */
  bool isMoveableObject(std::string id) const {
    if (_slide_deck.empty()) return false;
    return _slide_deck.at(_current_pos)->contains(id);
  }

  /**
   * Removes object from slides if moveable object
   */
  bool deleteItem(std::string id) {
    if (isMoveableObject(id)) {
      auto slide = _slide_deck.at(_current_pos);
      const std::vector<ImageLayout> images = slide->getImages();
      const auto imageCheck = std::ranges::find_if(
          images,
          [id](const ImageLayout& im) { return im.image->getID() == id; });
      if (imageCheck != images.end()) {
        _event_manager.removeEvent(id);
        slide->removeImage(slide->getImageFromID(id));
      } else {
        _event_manager.removeEvent(id);
        slide->removeTextBox(slide->getTextboxFromID(id));
      }
      std::cout << "Removed object: " << id << std::endl;
      return true;
    }
    return false;
  }

  /**
   * Getter for slides
   * @return slides in the deck
   */
  const std::vector<std::shared_ptr<WebLayout>>& getSlides() const {
    return _slide_deck;
  }

  /**
   * @brief Get the Current Position of the manager
   * @return int current position in the slide vector
   */
  int getCurrentPos() const { return _current_pos; }

  /**
   * This handles the edge case where a slide transition occurs while there are
   * remaining object events The objects still needs to reappear in the layout
   */
  void resetObjects() { _event_manager.resetObjects(); }

  /**
   * @brief Get the numbers of slides within the presentation
   */
  int getNumSlides() const { return static_cast<int>(_slide_deck.size()); }

  /**
   * Update html when number of slides or position has changed
   */
  void onSlideChangedJS() {
    EM_ASM(
        {
            const currentSlide = $0;
            const totalSlides = $1;
            document.getElementById("slideNumberDisplay").textContent =
            `${currentSlide} of ${totalSlides}`;
        },
        getCurrentPos() + 1, getNumSlides());
  }

  /**
   * Add a slide change transition
   * @param time in seconds
   * @param origin Origin slide
   * @param destination Destination slide
   */
  void addSlideChangeEvent(const int time, const int origin,
                           const int destination) {
    // Check if destination slide exists (destination count is 1 off for user
    // readability on UI)
    if (destination > getNumSlides() - 1 || getNumSlides() == 0) {
      std::cout << "WARNING: Destination slide " << (destination + 1)
                << " doesn't exist." << std::endl;
      return;
    }

    std::cout << "Added slide transition: Go from slide " << (origin + 1)
              << " to slide " << (destination + 1) << " after " << time
              << " seconds." << std::endl;
    _event_manager.addEvent(changeSlide, origin, std::to_string(destination),
                            time);
  }

  /**
   * Object events are when textbox or images appear after a certain time
   * They are actually two events: make the object invisible at time 0, and
   * visible at time "timing"
   * @param timing
   * @param slideNum
   * @param id
   */
  void addObjectEvent(const int timing, const int slideNum,
                      const std::string& id) {
    if (parseType(id) == EventType::Image) {  // Image
      std::cout << "Added image event: ID " << id << " appearing after "
                << timing << " seconds." << std::endl;
      _event_manager.addEvent(imageToggle, slideNum, id, timing);
    } else {  // Textbox
      std::cout << "Added textbox event: ID " << id << " appearing after "
                << timing << " seconds." << std::endl;
      _event_manager.addEvent(textBoxToggle, slideNum, id, timing);
    }
  }

  /**
   * Toggle an image on the slide on/off
   * @param id
   */
  void toggleImage(const std::string& id) const {
    const Slide slide = _slide_deck.at(_current_pos);
    const auto imageLayout = slide->getImageFromID(id);
    slide->toggleImage(imageLayout);
  }

  /**
   * Toggle a textbox on the slide on/off
   * @param id
   */
  void toggleTextbox(const std::string& id) const {
    const Slide slide = _slide_deck.at(_current_pos);
    const auto textBoxLayout = slide->getTextboxFromID(id);
    slide->toggleTextBox(textBoxLayout);
  }

  /**
   * Updates an image size
   * @param id of image
   * @param newWidth of image
   * @param newHeight of image
   */
  void updateImageSize(const std::string& id, int newWidth, int newHeight) {
    if (_slide_deck.empty() || _current_pos >= _slide_deck.size()) return;
    auto& layout = _slide_deck.at(_current_pos);

    for (auto& img : layout->getImages()) {
      if (img.image->getID() == id) {
        img.image->resize(newWidth, newHeight, false);
        break;
      }
    }
  }

  void toggleBottomNav(bool hidden) {
    // Hide bottom edit bar
    EM_ASM(
        {
          var bottomNav = document.getElementById("bottom-nav");
          if (bottomNav) {
            console.log('Present Mode: ', $0);
            bottomNav.style.visibility = $0 ? "hidden" : "visible";
          }
        },
        hidden);
  }

  /**
   * Toggles borders of textboxes/images during presentations
   * @param hidden
   */
  void toggleBorders(bool hidden) {
    // Hide bottom edit bar
    EM_ASM(
        {
          var images = document.getElementsByClassName("image");
          for (var i = 0; i < images.length; i++) {
            images[i].style.border = $0 ? "none" : "1px solid black";
            images[i].style.resize = $0 ? "none" : "both";
          }
          var textboxes = document.getElementsByClassName("textbox");
          for (var i = 0; i < textboxes.length; i++) {
            textboxes[i].style.border = $0 ? "none" : "1px solid black";
            textboxes[i].style.resize = $0 ? "none" : "both";
          }
        },
        hidden);
  }

  /**
   * Return event info on a given slide
   * @param slideNum
   * @return map of IDs to timings
   */
  std::unordered_map<std::string, int> getSlideEventInfo(
      const int slideNum) const {
    return _event_manager.getSlideEventInfo(slideNum);
  }

  bool isRunning() const { return _running; }

  void moveSlide(bool const forward) {
    // Check if any slides exist
    if (getNumSlides() == 0) {
      std::cout << "WARNING: No slides exist" << std::endl;
      return;
    }

    int destination = 0;
    if (forward) {
      // Check if is last slide
      if (_current_pos == getNumSlides() - 1) {
        std::cout << "WARNING: Already is last slide" << std::endl;
        return;
      }
      // Move forward one slide
      destination = _current_pos + 1;

    } else {
      // Check if is first slide
      if (_current_pos == 0) {
        std::cout << "WARNING: Already is first slide" << std::endl;
        return;
      }

      // Move back one slide
      destination = _current_pos - 1;
    }

    // Swap position
    std::swap(_slide_deck[_current_pos], _slide_deck[destination]);
    std::cout << "Swapped slides " << _current_pos << " and " << destination
              << std::endl;
    goTo(destination);
    onSlideChangedJS();
  }

  ////////////////////////////////////////////////////////////////////////////////////////
  /// STATIC EVENT FUNCTIONS
  /// These are passed to the Events to allow Events to change the layouts
  ////////////////////////////////////////////////////////////////////////////////////////
  static void changeSlide(PresentationManager* presentation_manager,
                          const int slideNum) {
    presentation_manager->resetObjects();
    presentation_manager->goTo(slideNum);
  }

  static void imageToggle(PresentationManager* presentation_manager,
                          const int id) {
    const auto str_id = "textbox-" + std::to_string(id);
    std::cout << "Triggering event: " << str_id << std::endl;
    presentation_manager->toggleImage("image-" + std::to_string(id));
  }

  static void textBoxToggle(PresentationManager* presentation_manager,
                            const int id) {
    const auto str_id = "textbox-" + std::to_string(id);
    std::cout << "Triggering event: " << str_id << std::endl;
    presentation_manager->toggleTextbox("textbox-" + std::to_string(id));
  }

  ////////////////////
  /// JSON
  ////////////////////
  static void loadSlideDeckFromJson(const char* jsonStr,
                                    PresentationManager* presentation_manager) {
    json deck = json::parse(jsonStr);
    presentation_manager->clear();  // Clear the deck before loading new slides
    presentation_manager->initialize(deck["slides"].size());  // Initialize
    std::vector<std::tuple<int, int, int>>
        transitionBuffer;  // Buffer so we can add transitions after all slides
                           // pushed
    int slide_num = 0;
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
          if (tbJson.contains("event")) {
            presentation_manager->addObjectEvent(tbJson["event"]["time"],
                                                 slide_num, textbox->getID());
          }
        }
      }

      // Load all images
      if (slide.contains("images")) {
        for (const auto& imgJson : slide["images"]) {
          auto image = std::make_shared<Image>(imgJson["url"], imgJson["width"],
                                               imgJson["height"]);
          layout->addImage(ImageLayout(image, imgJson["x"], imgJson["y"]));
          if (imgJson.contains("event")) {
            presentation_manager->addObjectEvent(imgJson["event"]["time"],
                                                 slide_num, image->getID());
          }
        }
      }
      presentation_manager->pushBackSlide(layout);  // Add to the global manager
      if (slide.contains("event")) {
        transitionBuffer.emplace_back(slide["event"]["time"], slide_num,
                                      slide_num + 1);
      }
      slide_num++;
    }
    for (auto t : transitionBuffer) {
      auto [time, origin, dest] = t;
      presentation_manager->addSlideChangeEvent(time, origin, dest);
    }
    presentation_manager->goTo(0);  // Show the first slide
  }

  static const char* exportSlideDeckToJson(
      const PresentationManager* presentation_manager) {
    static std::string exportedJson;
    json deck;
    deck["slides"] = json::array();
    int slide_num = 0;
    for (const auto& layout : presentation_manager->getSlides()) {
      json slide;
      const auto events = presentation_manager->getSlideEventInfo(slide_num);
      if (events.contains(std::to_string(slide_num + 1))) {
        slide["event"]["time"] = events.at(std::to_string(slide_num + 1));
        slide["event"]["dest"] = slide_num + 1;
      }

      slide["textBoxes"] = json::array();
      for (const auto& tb : layout->getTextBoxes()) {
        const auto& text = tb.textBox->getFormattedText();
        json tbJson = {{"text", text.getText()},
                       {"font", text.getFont()},
                       {"fontSize", text.getFontSize()},
                       {"color", text.getColor()},
                       {"width", tb.textBox->getWidth()},
                       {"height", tb.textBox->getHeight()},
                       {"x", tb.xPos},
                       {"y", tb.yPos}};
        if (events.contains(tb.textBox->getID())) {
          tbJson["event"]["time"] = events.at(tb.textBox->getID());
        }
        slide["textBoxes"].push_back(tbJson);
      }

      slide["images"] = json::array();
      for (const auto& img : layout->getImages()) {
        json imgJson = {{"url", img.image->getURL()},
                        {"width", img.image->getWidth()},
                        {"height", img.image->getHeight()},
                        {"x", img.xPos},
                        {"y", img.yPos}};
        if (events.contains(img.image->getID())) {
          imgJson["event"]["time"] = events.at(img.image->getID());
        }
        slide["images"].push_back(imgJson);
      }

      deck["slides"].push_back(slide);
      slide_num += 1;
    }

    exportedJson = deck.dump(2);  // pretty print
    return exportedJson.c_str();
  }
};

PresentationManager PRESENTATION_MANAGER;  ///< Global presentation manager

////////////////////////////////////////////////////////////////////////////////////////
/// Exported Emscripten functions
////////////////////////////////////////////////////////////////////////////////////////
extern "C" {
EMSCRIPTEN_KEEPALIVE
void call_stop() { PRESENTATION_MANAGER.stop(); }
void call_start() { PRESENTATION_MANAGER.start(); }
void call_advance() { PRESENTATION_MANAGER.advance(); }
void call_rewind() { PRESENTATION_MANAGER.rewind(); }
void call_addImage(char* urlPtr, int width, int height, char* altPtr) {
  const std::string url(urlPtr);
  const std::string altText(altPtr);
  PRESENTATION_MANAGER.addImage(url, width, height, altText);
}
void call_updateTextBoxContent(const char* textboxId, const char* newText) {
  if (PRESENTATION_MANAGER.getSlides().empty() ||
      PRESENTATION_MANAGER.getCurrentPos() >=
          PRESENTATION_MANAGER.getSlides().size()) {
    std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
    return;
  }

  auto& layout =
      PRESENTATION_MANAGER.getSlides().at(PRESENTATION_MANAGER.getCurrentPos());
  auto& textBoxes = layout->getTextBoxes();

  for (auto& tb : textBoxes) {
    if (tb.textBox->getID() == textboxId) {
      tb.textBox->getFormattedText().setText(newText);
      break;
    }
  }
}
void call_updateTextBoxFont(const char* textboxId, const char* newFont) {
  if (PRESENTATION_MANAGER.getSlides().empty() ||
      PRESENTATION_MANAGER.getCurrentPos() >=
          PRESENTATION_MANAGER.getSlides().size()) {
    std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
    return;
  }

  auto& layout =
      PRESENTATION_MANAGER.getSlides().at(PRESENTATION_MANAGER.getCurrentPos());
  auto& textBoxes = layout->getTextBoxes();

  for (auto& tb : textBoxes) {
    if (tb.textBox->getID() == textboxId) {
      tb.textBox->getFormattedText().setFont(newFont);
      break;
    }
  }
}

void call_updateTextBoxFontSize(const char* textboxId, int fontSize) {
  if (PRESENTATION_MANAGER.getSlides().empty() ||
      PRESENTATION_MANAGER.getCurrentPos() >=
          PRESENTATION_MANAGER.getSlides().size()) {
    std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
    return;
  }

  auto& layout =
      PRESENTATION_MANAGER.getSlides().at(PRESENTATION_MANAGER.getCurrentPos());
  auto& textBoxes = layout->getTextBoxes();

  for (auto& tb : textBoxes) {
    if (tb.textBox->getID() == textboxId) {
      tb.textBox->getFormattedText().setFontSize(fontSize);
      break;
    }
  }
}

void call_updateTextBoxColor(const char* textboxId, const char* newColor) {
  if (PRESENTATION_MANAGER.getSlides().empty() ||
      PRESENTATION_MANAGER.getCurrentPos() >=
          PRESENTATION_MANAGER.getSlides().size()) {
    std::cout << "ERROR: updateTextBoxContent(): layout index out of range.\n";
    return;
  }

  auto& layout =
      PRESENTATION_MANAGER.getSlides().at(PRESENTATION_MANAGER.getCurrentPos());
  auto& textBoxes = layout->getTextBoxes();

  for (auto& tb : textBoxes) {
    if (tb.textBox->getID() == textboxId) {
      tb.textBox->getFormattedText().setColor(newColor);
      break;
    }
  }
}

void call_updatePosition(const char* id, int newX, int newY) {
  std::string cppId(id);
  PRESENTATION_MANAGER.updatePosition(cppId, newX, newY);
}
void call_updateSize(const char* id, int newWidth, int newHeight) {
  std::string cppId(id);
  PRESENTATION_MANAGER.updateSize(cppId, newWidth, newHeight);
}
void call_addTextBox() { PRESENTATION_MANAGER.addTextBox(); }
void call_addNewSlide() { PRESENTATION_MANAGER.addNewSlide(); }
bool call_isMoveableObject(const char* id) {
  std::string cppId(id);
  return PRESENTATION_MANAGER.isMoveableObject(id);
}
void call_loadSlideDeckFromJson(const char* jsonStr) {
  PresentationManager::loadSlideDeckFromJson(jsonStr, &PRESENTATION_MANAGER);
}
const char* call_exportSlideDeckToJson() {
  return PresentationManager::exportSlideDeckToJson(&PRESENTATION_MANAGER);
}

void call_addSlideChangeEvent(const int time) {
  PRESENTATION_MANAGER.addSlideChangeEvent(
      time, PRESENTATION_MANAGER.getCurrentPos(),
      PRESENTATION_MANAGER.getCurrentPos() + 1);
}

void call_updateImageSize(const char* id, int width, int height) {
  std::string cppId(id);
  PRESENTATION_MANAGER.updateImageSize(cppId, width, height);
}

void call_leavePresentation() {
  PRESENTATION_MANAGER.toggleBottomNav(false);
  PRESENTATION_MANAGER.toggleBorders(false);

  // Ensure end screen is disabled
  PRESENTATION_MANAGER.toggleEndScreen(false);
  PRESENTATION_MANAGER.stop();
  PRESENTATION_MANAGER.goTo(PRESENTATION_MANAGER.getCurrentPos());
}

void call_nextEvent() {
  if (PRESENTATION_MANAGER.isRunning()) {
    PRESENTATION_MANAGER.next();
  }
}

void call_moveSlide(const bool forward) {
  PRESENTATION_MANAGER.moveSlide(forward);
}

void call_addObjectEvent(const int timing, const int slideNum, const char* id) {
  PRESENTATION_MANAGER.addObjectEvent(timing, slideNum, id);
}
int call_getCurrentPos() { return PRESENTATION_MANAGER.getCurrentPos(); }
bool call_deleteItem(const char* id) {
  std::string cppId(id);
  return PRESENTATION_MANAGER.deleteItem(cppId);
}
void call_deleteSlide() {
  // Delete only if more than 1 slide
  if (PRESENTATION_MANAGER.getSlides().empty() ||
      PRESENTATION_MANAGER.getCurrentPos() >=
          PRESENTATION_MANAGER.getSlides().size()) {
    std::cout << "ERROR: call_deleteSlide(): layout index out of range.\n";
    return;
  }

  if (PRESENTATION_MANAGER.getNumSlides() <= 1) {
    std::cout << "ERROR: call_deleteSlide(): can't delete only slide.\n";
    return;
  }

  auto layout =
      PRESENTATION_MANAGER.getSlides().at(PRESENTATION_MANAGER.getCurrentPos());

  PRESENTATION_MANAGER.getCurrentPos() == 0 ? PRESENTATION_MANAGER.advance()
                                            : PRESENTATION_MANAGER.rewind();

  PRESENTATION_MANAGER.deleteSlide(layout);
  PRESENTATION_MANAGER.onSlideChangedJS();
}
}
