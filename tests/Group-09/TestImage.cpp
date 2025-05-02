/**
* @file TestImage.cpp
 *
 * @author Mia Bodenhorn
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "MockClasses/MockImage.cpp"

using namespace cse;

TEST_CASE("Valid Image", "[Image]") {
 MockImage img("https://cse498.github.io/assets/img/logo.png", 400, 300, "Test Image");

 CHECK(img.getURL() == "https://cse498.github.io/assets/img/logo.png");
 CHECK(img.getWidth() == 400);
 CHECK(img.getHeight() == 300);
 CHECK(img.getAltText() == "Test Image");
}

TEST_CASE("Test invalid URL setURL", "[Image]") {
 MockImage img("http://example.com", 200, 100, "Test Image");

 REQUIRE_THROWS_AS(img.setURL("ftp://example.com"), std::invalid_argument);

 REQUIRE_THROWS_AS(img.setURL(""), std::invalid_argument);

}

TEST_CASE("Test resize with maintainAspect=true, newWidth < newHeight", "[Image]") {
 MockImage img("https://cse498.github.io/assets/img/logo.png", 400, 200, "Test Image");

 // Resize with maintainAspect=true, newWidth < newHeight
 img.resize(300, 500, true);

 // The aspect ratio is 400 / 200 = 2.0
 // With newHeight = 500, newWidth should be 500 * 2.0 = 1000
 // But since we specified newWidth = 300, the height should adjust to 300 / 2 = 150

 CHECK(img.getWidth() == 300);
 CHECK(img.getHeight() == 150);
}

