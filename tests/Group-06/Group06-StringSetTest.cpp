#include <string>
#include <iostream>
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-06/StringSet/StringSet.hpp"
#include "../../Group-06/StaticString/StaticString.hpp"

using namespace cse;

TEST_CASE("Insert and Size Tests", "[insert]"){
    StringSet set;

    REQUIRE(set.size() == 0);

    set.insert("blue");
    REQUIRE(set.size() == 1);

    set.insert("yellow");
    set.insert("black");
    REQUIRE(set.size() == 3);

    // Insert a duplicate, should not change size
    set.insert("black");
    REQUIRE(set.size() == 3);

    set.insert("purple");
    REQUIRE(set.size()==4);

    set.clear();
    set.insert({"black", "blue", "purple", "green", "yellow"});
    REQUIRE(set.size() == 5);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    REQUIRE(Sset.size() == 0);

    Sset.insert(StaticString<10>("blue"));
    REQUIRE(Sset.size() == 1);

    Sset.insert(StaticString<10>("yellow"));
    Sset.insert(StaticString<10>("black"));
    REQUIRE(Sset.size() == 3);

    // Insert a duplicate, should not change size
    Sset.insert(StaticString<10>("black"));
    REQUIRE(Sset.size() == 3);

    Sset.insert(StaticString<10>("purple"));
    REQUIRE(Sset.size()==4);

    Sset.clear();
    Sset.insert({StaticString<10>("black"), StaticString<10>("blue"), StaticString<10>("purple"), StaticString<10>("green"), StaticString<10>("yellow")});
    REQUIRE(Sset.size() == 5);
}

TEST_CASE("Test for erase", "[erase]"){
    StringSet set;

    set.insert("blue");
    set.insert("yellow");
    set.insert("black");
    REQUIRE(set.size() == 3);

    set.erase("yellow");
    set.erase("black");
    REQUIRE(set.size() == 1);

    // green not in the set, try deleting it, it should not affect our set
    set.erase("green");
    REQUIRE(set.size() == 1);

    set.erase("blue");
    REQUIRE(set.size() == 0);

    set.erase("white");
    REQUIRE(set.size()==0);


    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    Sset.insert(StaticString<10>("blue"));
    Sset.insert(StaticString<10>("yellow"));
    Sset.insert(StaticString<10>("black"));
    REQUIRE(Sset.size() == 3);

    Sset.erase(StaticString<10>("yellow"));
    Sset.erase(StaticString<10>("black"));
    REQUIRE(Sset.size() == 1);

    // green not in the set, try deleting it, it should not affect our set
    Sset.erase(StaticString<10>("green"));
    REQUIRE(Sset.size() == 1);

    Sset.erase(StaticString<10>("blue"));
    REQUIRE(Sset.size() == 0);

    Sset.erase(StaticString<10>("white"));
    REQUIRE(Sset.size()==0);

}


TEST_CASE("Test for clear", "[clear]"){

    StringSet set;

    set.insert("blue");
    set.insert("yellow");
    REQUIRE(set.size() == 2);

    set.clear();
    REQUIRE(set.size() == 0);

    // Try clearing empty set
    set.clear();
    REQUIRE(set.size() == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    Sset.insert(StaticString<10>("blue"));
    Sset.insert(StaticString<10>("yellow"));
    REQUIRE(Sset.size() == 2);

    Sset.clear();
    REQUIRE(Sset.size() == 0);

    // Try clearing empty set
    Sset.clear();
    REQUIRE(Sset.size() == 0);

}

TEST_CASE("More test for erase and clear together", "[erase][clear]"){
    StringSet set;

    set.insert("white");
    set.insert("blue");
    REQUIRE(set.size() == 2);

    set.erase("white");
    set.erase("blue");
    REQUIRE(set.size() == 0);

    // Try clear in empty set
    set.clear();
    REQUIRE(set.size() == 0);

    // Try deleting element from empty set
    set.erase("yellow");
    REQUIRE(set.size() == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    Sset.insert(StaticString<10>("white"));
    Sset.insert(StaticString<10>("blue"));
    REQUIRE(Sset.size() == 2);

    Sset.erase(StaticString<10>("white"));
    Sset.erase(StaticString<10>("blue"));
    REQUIRE(Sset.size() == 0);

    // Try clear in empty set
    Sset.clear();
    REQUIRE(Sset.size() == 0);

    // Try deleting element from empty set
    Sset.erase(StaticString<10>("yellow"));
    REQUIRE(Sset.size() == 0);

}

TEST_CASE("Test for empty", "[empty]"){
    StringSet set;
    REQUIRE(set.empty());

    set.insert("yellow");
    REQUIRE_FALSE(set.empty());

    set.erase("yellow");
    REQUIRE(set.empty());

    set.insert("blue");
    set.clear();
    REQUIRE(set.empty());

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;
    REQUIRE(Sset.empty());

    Sset.insert(StaticString<10>("yellow"));
    REQUIRE_FALSE(Sset.empty());

    Sset.erase(StaticString<10>("yellow"));
    REQUIRE(Sset.empty());

    Sset.insert(StaticString<10>("blue"));
    Sset.clear();
    REQUIRE(Sset.empty());

}

TEST_CASE("Test for count", "[count]"){
    StringSet set;

    // Empty set
    REQUIRE(set.count("blue") == 0);

    set.insert("blue");
    set.insert("yellow");
    REQUIRE(set.count("blue") == 1);
    REQUIRE(set.count("yellow") == 1);
    REQUIRE(set.count("red") == 0);

    // Try adding existing element
    set.insert("blue");
    REQUIRE(set.count("blue") == 1);

    // Check after erase
    set.erase("blue");
    REQUIRE(set.count("blue") == 0);

    // Check after clear
    set.clear();
    REQUIRE(set.count("yellow") == 0);
    REQUIRE(set.empty());


    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    // Empty set
    REQUIRE(Sset.count(StaticString<10>("blue")) == 0);

    Sset.insert(StaticString<10>("blue"));
    Sset.insert(StaticString<10>("yellow"));
    REQUIRE(Sset.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset.count(StaticString<10>("red")) == 0);

    // Try adding existing element
    Sset.insert(StaticString<10>("blue"));
    REQUIRE(Sset.count(StaticString<10>("blue")) == 1);

    // Check after erase
    Sset.erase(StaticString<10>("blue"));
    REQUIRE(Sset.count(StaticString<10>("blue")) == 0);

    // Check after clear
    Sset.clear();
    REQUIRE(Sset.count(StaticString<10>("yellow")) == 0);
    REQUIRE(Sset.empty());
}

TEST_CASE("Test for Union", "[Union]"){
    StringSet set1, set2;

    // Empty sets
    StringSet unionSet = set1.Union(set2);
    REQUIRE(unionSet.size() == 0);
    REQUIRE(set1.size() == 0);
    REQUIRE(set2.size() == 0);

    // Not empty sets
    set1.insert("blue");
    set1.insert("yellow");
    set1.insert("black");
    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("black") == 1);

    set2.insert("green");
    set2.insert("pink");
    set2.insert("black");
    set2.insert("green");
    REQUIRE(set2.size() == 3);
    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("pink") == 1);
    REQUIRE(set2.count("black") == 1);

    unionSet = set1.Union(set2);
    REQUIRE(unionSet.count("blue") == 1);
    REQUIRE(unionSet.count("yellow") == 1);
    REQUIRE(unionSet.count("black") == 1);
    REQUIRE(unionSet.count("green") == 1);
    REQUIRE(unionSet.count("pink") == 1);
    REQUIRE(unionSet.count("white") == 0);
    REQUIRE(unionSet.size() == 5); // blue, yellow, black, green, pink

    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("pink") == 1);
    REQUIRE(set2.count("black") == 1);

    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("black") == 1);

    // One set empty other one is not
    set1.clear();
    unionSet = set1.Union(set2);
    REQUIRE(unionSet.count("blue") == 0);
    REQUIRE(unionSet.count("yellow") == 0);
    REQUIRE(unionSet.count("black") == 1);
    REQUIRE(unionSet.count("green") == 1);
    REQUIRE(unionSet.count("pink") == 1);
    REQUIRE(unionSet.count("white") == 0);
    REQUIRE(unionSet.size() == 3); // black, green, pink

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset1, Sset2;

    // Empty sets
    StringSet<StaticString<10>> SunionSet = Sset1.Union(Sset2);
    REQUIRE(SunionSet.size() == 0);
    REQUIRE(Sset1.size() == 0);
    REQUIRE(Sset2.size() == 0);

    // Not empty sets
    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("yellow"));
    Sset1.insert(StaticString<10>("black"));
    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);

    Sset2.insert(StaticString<10>("green"));
    Sset2.insert(StaticString<10>("pink"));
    Sset2.insert(StaticString<10>("black"));
    Sset2.insert(StaticString<10>("green"));
    REQUIRE(Sset2.size() == 3);
    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("pink")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("black")) == 1);

    SunionSet = Sset1.Union(Sset2);
    REQUIRE(SunionSet.count(StaticString<10>("blue")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("yellow")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("black")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("green")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("pink")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("white")) == 0);
    REQUIRE(SunionSet.size() == 5); // blue, yellow, black, green, pink

    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("pink")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("black")) == 1);

    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);

    // One set empty other one is not
    Sset1.clear();
    SunionSet = Sset1.Union(Sset2);
    REQUIRE(SunionSet.count(StaticString<10>("blue")) == 0);
    REQUIRE(SunionSet.count(StaticString<10>("yellow")) == 0);
    REQUIRE(SunionSet.count(StaticString<10>("black")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("green")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("pink")) == 1);
    REQUIRE(SunionSet.count(StaticString<10>("white")) == 0);
    REQUIRE(SunionSet.size() == 3); // black, green, pink

}


TEST_CASE("Test for Intersection", "[Intersection]"){
    StringSet set1, set2;

    // Empty sets
    StringSet intersectionSet = set1.Intersection(set2);
    REQUIRE(intersectionSet.size() == 0);
    REQUIRE(set1.size() == 0);
    REQUIRE(set2.size() == 0);

    // Not empty sets
    set1.insert("blue");
    set1.insert("yellow");
    set1.insert("black");
    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("black") == 1);

    set2.insert("green");
    set2.insert("blue");
    set2.insert("black");
    set2.insert("green");
    REQUIRE(set2.size() == 3);
    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("blue") == 1);
    REQUIRE(set2.count("black") == 1);

    intersectionSet = set1.Intersection(set2);
    REQUIRE(intersectionSet.count("blue") == 1);
    REQUIRE(intersectionSet.count("black") == 1);
    REQUIRE(intersectionSet.count("green") == 0);
    REQUIRE(intersectionSet.count("yellow") == 0);
    REQUIRE(intersectionSet.size() == 2); // blue, black

    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("blue") == 1);
    REQUIRE(set2.count("black") == 1);

    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("black") == 1);


    // One empty set
    set1.clear();
    intersectionSet = set1.Intersection(set2);
    REQUIRE(intersectionSet.count("blue") == 0);
    REQUIRE(intersectionSet.count("black") == 0);
    REQUIRE(intersectionSet.size() == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset1, Sset2;

    // Empty sets
    StringSet<StaticString<10>> SintersectionSet = Sset1.Intersection(Sset2);
    REQUIRE(SintersectionSet.size() == 0);
    REQUIRE(Sset1.size() == 0);
    REQUIRE(Sset2.size() == 0);

    // Not empty sets
    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("yellow"));
    Sset1.insert(StaticString<10>("black"));
    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);

    Sset2.insert(StaticString<10>("green"));
    Sset2.insert(StaticString<10>("blue"));
    Sset2.insert(StaticString<10>("black"));
    Sset2.insert(StaticString<10>("green"));
    REQUIRE(Sset2.size() == 3);
    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("black")) == 1);

    SintersectionSet = Sset1.Intersection(Sset2);
    REQUIRE(SintersectionSet.count(StaticString<10>("blue")) == 1);
    REQUIRE(SintersectionSet.count(StaticString<10>("black")) == 1);
    REQUIRE(SintersectionSet.count(StaticString<10>("green")) == 0);
    REQUIRE(SintersectionSet.count(StaticString<10>("yellow")) == 0);
    REQUIRE(SintersectionSet.size() == 2); // blue, black

    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("black")) == 1);

    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);


    // One empty set
    Sset1.clear();
    SintersectionSet = Sset1.Intersection(Sset2);
    REQUIRE(SintersectionSet.count(StaticString<10>("blue")) == 0);
    REQUIRE(SintersectionSet.count(StaticString<10>("black")) == 0);
    REQUIRE(SintersectionSet.size() == 0);

}


TEST_CASE("Test for Difference", "[Difference]"){
    StringSet set1, set2;

    set1.insert("blue");
    set1.insert("yellow");
    set1.insert("black");
    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("black") == 1);


    set2.insert("green");
    set2.insert("blue");
    set2.insert("purple");
    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("blue") == 1);
    REQUIRE(set2.count("purple") == 1);


    StringSet differenceSet1 = set1.Difference(set2);
    REQUIRE(differenceSet1.count("yellow") == 1);
    REQUIRE(differenceSet1.count("black") == 1);
    REQUIRE(differenceSet1.count("blue") == 0);
    REQUIRE_FALSE(differenceSet1.count("green") == 1);
    REQUIRE(differenceSet1.size() == 2); // yellow, black (elements that present in first set but not in second set)

    StringSet differenceSet2 = set2.Difference(set1);
    REQUIRE(differenceSet2.count("green") == 1);
    REQUIRE(differenceSet2.count("purple") == 1);
    REQUIRE(differenceSet2.count("blue") == 0);
    REQUIRE_FALSE(differenceSet2.count("yellow") == 1);
    REQUIRE(differenceSet2.size() == 2); // green and purple (present in set2 but not in set1)

    REQUIRE(set1.count("blue") == 1);
    REQUIRE(set1.count("yellow") == 1);
    REQUIRE(set1.count("black") == 1);

    REQUIRE(set2.count("green") == 1);
    REQUIRE(set2.count("blue") == 1);
    REQUIRE(set2.count("purple") == 1);

    // Try same set, needs to return empty set
    StringSet differenceSet3 = set2.Difference(set2);
    REQUIRE(differenceSet3.size() == 0);

    set1.clear();
    set2.clear();
    differenceSet1.clear();
    differenceSet2.clear();
    REQUIRE(set1.size() == 0);
    REQUIRE(set2.size() == 0);
    REQUIRE(differenceSet1.size() == 0);
    REQUIRE(differenceSet2.size() == 0);

    // 2 sets with the same elements
    set1.insert("white");
    set1.insert("brown");
    set1.insert("red");

    set2.insert("white");
    set2.insert("brown");
    set2.insert("red");

    differenceSet1 = set1.Difference(set2);
    REQUIRE(differenceSet1.count("white") == 0);
    REQUIRE(differenceSet1.count("brown") == 0);
    REQUIRE(differenceSet1.count("red") == 0);
    REQUIRE_FALSE(differenceSet1.count("yellow") == 1);
    REQUIRE(differenceSet1.size() == 0);

    differenceSet2 = set2.Difference(set1);
    REQUIRE(differenceSet2.count("white") == 0);
    REQUIRE(differenceSet2.count("brown") == 0);
    REQUIRE(differenceSet2.count("red") == 0);
    REQUIRE_FALSE(differenceSet2.count("green") == 1);
    REQUIRE(differenceSet2.size() == 0);

    // 2 empty sets
    set1.clear();
    set2.clear();

    differenceSet1 = set1.Difference(set2);
    REQUIRE(differenceSet1.count("white") == 0);
    REQUIRE(differenceSet1.size() == 0);

    differenceSet2 = set2.Difference(set1);
    REQUIRE(differenceSet2.count("white") == 0);
    REQUIRE(differenceSet2.size() == 0);

    // one set empty other is not
    set2.insert("blue");
    differenceSet2 = set2.Difference(set1);
    REQUIRE(differenceSet2.size() == 1);
    REQUIRE(differenceSet2.count("blue") == 1);
    REQUIRE(differenceSet2.count("black") == 0);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset1, Sset2;

    Sset1.insert(StaticString<10>("blue"));
    Sset1.insert(StaticString<10>("yellow"));
    Sset1.insert(StaticString<10>("black"));
    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);


    Sset2.insert(StaticString<10>("green"));
    Sset2.insert(StaticString<10>("blue"));
    Sset2.insert(StaticString<10>("purple"));
    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("purple")) == 1);


    StringSet<StaticString<10>> SdifferenceSet1 = Sset1.Difference(Sset2);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("black")) == 1);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("blue")) == 0);
    REQUIRE_FALSE(SdifferenceSet1.count(StaticString<10>("green")) == 1);
    REQUIRE(SdifferenceSet1.size() == 2); // yellow, black (elements that present in first set but not in second set)

    StringSet<StaticString<10>> SdifferenceSet2 = Sset2.Difference(Sset1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("green")) == 1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("purple")) == 1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("blue")) == 0);
    REQUIRE_FALSE(SdifferenceSet2.count(StaticString<10>("yellow")) == 1);
    REQUIRE(SdifferenceSet2.size() == 2); // green and purple (present in set2 but not in set1)

    REQUIRE(Sset1.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset1.count(StaticString<10>("black")) == 1);

    REQUIRE(Sset2.count(StaticString<10>("green")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("blue")) == 1);
    REQUIRE(Sset2.count(StaticString<10>("purple")) == 1);

    // Try same set, needs to return empty set
    StringSet<StaticString<10>> SdifferenceSet3 = Sset2.Difference(Sset2);
    REQUIRE(SdifferenceSet3.size() == 0);

    Sset1.clear();
    Sset2.clear();
    SdifferenceSet1.clear();
    SdifferenceSet2.clear();
    REQUIRE(Sset1.size() == 0);
    REQUIRE(Sset2.size() == 0);
    REQUIRE(SdifferenceSet1.size() == 0);
    REQUIRE(SdifferenceSet2.size() == 0);

    // 2 sets with the same elements
    Sset1.insert(StaticString<10>("white"));
    Sset1.insert(StaticString<10>("brown"));
    Sset1.insert(StaticString<10>("red"));

    Sset2.insert(StaticString<10>("white"));
    Sset2.insert(StaticString<10>("brown"));
    Sset2.insert(StaticString<10>("red"));

    SdifferenceSet1 = Sset1.Difference(Sset2);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("white")) == 0);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("brown")) == 0);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("red")) == 0);
    REQUIRE_FALSE(SdifferenceSet1.count(StaticString<10>("yellow")) == 1);
    REQUIRE(SdifferenceSet1.size() == 0);

    SdifferenceSet2 = Sset2.Difference(Sset1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("white")) == 0);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("brown")) == 0);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("red")) == 0);
    REQUIRE_FALSE(SdifferenceSet2.count(StaticString<10>("green")) == 1);
    REQUIRE(SdifferenceSet2.size() == 0);

    // 2 empty sets
    Sset1.clear();
    Sset2.clear();

    SdifferenceSet1 = Sset1.Difference(Sset2);
    REQUIRE(SdifferenceSet1.count(StaticString<10>("white")) == 0);
    REQUIRE(SdifferenceSet1.size() == 0);

    SdifferenceSet2 = Sset2.Difference(Sset1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("white")) == 0);
    REQUIRE(SdifferenceSet2.size() == 0);

    // one set empty other is not
    Sset2.insert(StaticString<10>("blue"));
    SdifferenceSet2 = Sset2.Difference(Sset1);
    REQUIRE(SdifferenceSet2.size() == 1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("blue")) == 1);
    REQUIRE(SdifferenceSet2.count(StaticString<10>("black")) == 0);

}


 TEST_CASE("Test for RetainFilter ", "[RetainFilter]"){

     StringSet set;

     set.insert("blue");
     set.insert("yellow");
     set.insert("black");
     set.insert("red");
     set.insert("brown");
     set.insert("white");

     set.RetainFilter([](const std::string& str){
         return str.size() > 4;
     });

     REQUIRE(set.size() == 4);
     REQUIRE(set.count("yellow") == 1);
     REQUIRE(set.count("black") == 1);
     REQUIRE(set.count("brown") == 1);
     REQUIRE(set.count("white") == 1);
     REQUIRE(set.count("blue") == 0);
     REQUIRE(set.count("red") == 0);
     REQUIRE(set.count("green") == 0);

     set.clear();
     REQUIRE(set.size() == 0);
     REQUIRE(set.count("yellow") == 0);

     // Try in empty set (shouldn't give any errors, set still needs to be empty)
     set.RetainFilter([](const std::string& str){
         return str.size() > 4;
     });

     REQUIRE(set.size() == 0);

     // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    Sset.insert(StaticString<10>("blue"));
    Sset.insert(StaticString<10>("yellow"));
    Sset.insert(StaticString<10>("black"));
    Sset.insert(StaticString<10>("red"));
    Sset.insert(StaticString<10>("brown"));
    Sset.insert(StaticString<10>("white"));

    Sset.RetainFilter([](const StaticString<10>& str){
        return str.length() > 4;
    });

    REQUIRE(Sset.size() == 4);
    REQUIRE(Sset.count(StaticString<10>("yellow")) == 1);
    REQUIRE(Sset.count(StaticString<10>("black")) == 1);
    REQUIRE(Sset.count(StaticString<10>("brown")) == 1);
    REQUIRE(Sset.count(StaticString<10>("white")) == 1);
    REQUIRE(Sset.count(StaticString<10>("blue")) == 0);
    REQUIRE(Sset.count(StaticString<10>("red")) == 0);
    REQUIRE(Sset.count(StaticString<10>("green")) == 0);

    Sset.clear();
    REQUIRE(Sset.size() == 0);
    REQUIRE(Sset.count(StaticString<10>("yellow")) == 0);

    // Try in empty set (shouldn't give any errors, set still needs to be empty)
    Sset.RetainFilter([](const StaticString<10>& str){
        return str.length() > 4;
    });

    REQUIRE(Sset.size() == 0);


}

 TEST_CASE("Test for RemoveFilter", "[RemoveFilter]"){
     StringSet set;

     set.insert("blue");
     set.insert("yellow");
     set.insert("black");
     set.insert("red");
     set.insert("brown");
     set.insert("white");


     set.RemoveFilter([](const std::string& str){
         return str.size() > 4;
     });

     REQUIRE(set.size() == 2);
     REQUIRE(set.count("yellow") == 0);
     REQUIRE(set.count("black") == 0);
     REQUIRE(set.count("brown") == 0);
     REQUIRE(set.count("white") == 0);
     REQUIRE(set.count("blue") == 1);
     REQUIRE(set.count("red") == 1);
     REQUIRE(set.count("green") == 0);

     set.clear();
     REQUIRE(set.size() == 0);
     REQUIRE(set.count("yellow") == 0);


     // Try it in an empty set (shouldn't give any errors, still needs to be empty set)
     set.RemoveFilter([](const std::string& str){
         return str.size() > 4;
     });

     REQUIRE(set.size() == 0);

     // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    Sset.insert("blue");
    Sset.insert("yellow");
    Sset.insert("black");
    Sset.insert("red");
    Sset.insert("brown");
    Sset.insert("white");


    Sset.RemoveFilter([](const StaticString<10>& str){
        return str.length() > 4;
    });

    REQUIRE(Sset.size() == 2);
    REQUIRE(Sset.count("yellow") == 0);
    REQUIRE(Sset.count("black") == 0);
    REQUIRE(Sset.count("brown") == 0);
    REQUIRE(Sset.count("white") == 0);
    REQUIRE(Sset.count("blue") == 1);
    REQUIRE(Sset.count("red") == 1);
    REQUIRE(Sset.count("green") == 0);

    Sset.clear();
    REQUIRE(Sset.size() == 0);
    REQUIRE(Sset.count("yellow") == 0);


    // Try it in an empty set (shouldn't give any errors, still needs to be empty set)
    Sset.RemoveFilter([](const StaticString<10>& str){
        return str.length() > 4;
    });

    REQUIRE(Sset.size() == 0);


}



TEST_CASE("Test for iterator", "[iterator]"){

    StringSet set;

    REQUIRE(set.size() == 0);
    REQUIRE(set.begin() == set.end());

    set.insert("blue");
    set.insert("yellow");
    set.insert("black");
    set.insert("green");
    set.insert("white");

    REQUIRE(set.size() == 5);
    REQUIRE(set.begin() != set.end());

    std::unordered_set<std::string> expected = {"blue", "yellow", "black", "green", "white"};
    std::unordered_set<std::string> iterated;

    for(auto i = set.begin(); i != set.end(); ++i)
    {
        iterated.insert(*i);
    }

    REQUIRE(iterated.size() == 5);
    REQUIRE(expected == iterated);

    // TEST FOR STATICSTRING

    StringSet<StaticString<10>> Sset;

    REQUIRE(Sset.size() == 0);
    REQUIRE(Sset.begin() == Sset.end());

    Sset.insert(StaticString<10>("blue"));
    Sset.insert(StaticString<10>("yellow"));
    Sset.insert(StaticString<10>("black"));
    Sset.insert(StaticString<10>("green"));
    Sset.insert(StaticString<10>("white"));

    REQUIRE(Sset.size() == 5);
    REQUIRE(Sset.begin() != Sset.end());

    std::unordered_set<StaticString<10>> STexpected = {StaticString<10>("blue"), StaticString<10>("yellow"),
            StaticString<10>("black"), StaticString<10>("green"), StaticString<10>("white")};
    std::unordered_set<StaticString<10>> Siterated;

    for(auto i = Sset.begin(); i != Sset.end(); ++i)
    {
        Siterated.insert(*i);
    }

    REQUIRE(Siterated.size() == 5);
    REQUIRE(STexpected == Siterated);

}

