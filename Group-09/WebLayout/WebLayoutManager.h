/**
 * Manager for holding and interacting with various WebLayouts
 *
 * @file WebLayoutManager.h
 * @author Mary Holt
 *
 */

#include <vector>
#include "WebLayout.h"

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
  void initialize();
  WebLayoutManager();
};

}