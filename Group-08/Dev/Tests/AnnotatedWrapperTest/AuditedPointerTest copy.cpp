#include "../../../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../AnnotatedWrapper/AnnotatedWrapper.h"

TEST_CASE("Test cse::AnnotatedWrapper", "[base]")
{
    AnnotatedWrapper wrapper;
    wrapper.addAnnotation("key1", "value1");
    wrapper.addAnnotation("key2", "value2");
    wrapper.addAnnotation("key3", "value3");

    SECTION("Check if annotations are added correctly")
    {
        REQUIRE(wrapper.getAnnotation("key1") == "value1");
        REQUIRE(wrapper.getAnnotation("key2") == "value2");
        REQUIRE(wrapper.getAnnotation("key3") == "value3");
    }

    SECTION("Check if annotations are removed correctly")
    {
        wrapper.removeAnnotation("key2");
        REQUIRE(wrapper.getAnnotation("key2") == "");
    }

}