/**
* @file TestApplication.cpp
 *
 * @author Grace Fitzgerald
 */

#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "MockClasses/MockPresentationManager.hpp"

using namespace cse;

TEST_CASE("Initial state is correct","[PresentationManager]") {
 MockPresentationManager mPm;
 REQUIRE(mPm.getNumSlides() == 0);
 REQUIRE(mPm.getCurrentPos() == 0);
 REQUIRE(mPm.isRunning() == false);

}



