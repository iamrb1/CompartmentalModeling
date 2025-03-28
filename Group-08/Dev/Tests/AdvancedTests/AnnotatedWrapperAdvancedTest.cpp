#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../AnnotatedWrapper/AnnotatedWrapper.hpp"
#include <sstream>

using namespace cse;

TEST_CASE("Test AnnotatedWrapper listAnnotations with lambda", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;
    wrapper.addAnnotation("language", "C++");
    wrapper.addAnnotation("editor", "VSCode");

    std::unordered_map<std::string, std::string> captured;
    wrapper.listAnnotations([&captured](const std::string& key, const std::string& value) {
        captured[key] = value;
    });

    CHECK(captured.size() == 2);
    CHECK(captured["language"] == "C++");
    CHECK(captured["editor"] == "VSCode");
}

TEST_CASE("Test overwriting an existing annotation", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;
    wrapper.addAnnotation("theme", "light");
    wrapper.addAnnotation("theme", "dark");

    CHECK(wrapper.getAnnotation("theme") == "dark");
}

TEST_CASE("Test getAnnotation with missing key", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;

    CHECK(wrapper.getAnnotation("missingKey").empty());
}

TEST_CASE("Test clearAnnotations on empty wrapper", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;

    // should not throw or crash
    wrapper.clearAnnotations();

    CHECK(wrapper.getAnnotation("any").empty());
}

TEST_CASE("Test AnnotatedWrapper with float values", "[AnnotatedWrapper][Template]") {
    cse::AnnotatedWrapper<float> wrapper;
    wrapper.addAnnotation("opacity", 0.75f);
    wrapper.addAnnotation("scale", 1.5f);

    CHECK(wrapper.getAnnotation("opacity") == Approx(0.75f));
    CHECK(wrapper.getAnnotation("scale") == Approx(1.5f));
}

TEST_CASE("Test set and get font size", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;
    wrapper.setFontSize(14);
    CHECK(wrapper.getFontSize() == 14);

    wrapper.setFontSize(24);
    CHECK(wrapper.getFontSize() == 24);
}

TEST_CASE("Test set and get font color", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;
    wrapper.setFontColor(255);
    CHECK(wrapper.getFontColor() == 255);
}

TEST_CASE("Test set and get background color", "[AnnotatedWrapper]") {
    cse::AnnotatedWrapper<std::string> wrapper;
    wrapper.setBackgroundColor(123);
    CHECK(wrapper.getBackgroundColor() == 123);
}