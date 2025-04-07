/**
 * Manager for holding and interacting with various WebLayouts
 *
 * @file WebLayoutManager.h
 * @author Mary Holt
 *
 */

#include <vector>

#include "WebLayout.hpp"

namespace cse {

class WebLayoutManager {
 private:
  int currentPos = 0;
  std::vector<std::shared_ptr<WebLayout>> layouts;

 public:
  void addLayout(std::shared_ptr<WebLayout> layout);
  void removeLayout(std::shared_ptr<WebLayout> layout);
  void advance();
  void rewind();
  void goTo(size_t slide_num);
  void initialize();
  void addTextBox();
  void addNewSlide();
  void addImage(const std::string& url, int width, int height,
                const std::string& altText);

  WebLayoutManager();

  // Getter for export support
  const std::vector<std::shared_ptr<WebLayout>>& getLayouts() const {
    return layouts;
  }

  /**
   * @brief Get the Current Position of the manager
   * @return int current position in the layout vector
   */
  const int getCurrentPos() const { return currentPos; }
};

}  // namespace cse