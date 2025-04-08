#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Dev/AuditedPointer/AuditedPointer.hpp"

TEST_CASE("Aptr Basic Operations", "[Aptr][Basic]") 
{
    SECTION("Default Construction") {
        cse::Aptr<int> ptr;
        CHECK(!ptr);
        CHECK(ptr.GetID() > 0);
        ptr.Delete();
    }

    SECTION("Value Construction") {
        cse::Aptr<int> ptr = cse::MakeAudited<int>(42);
        CHECK(*ptr == 42);
        CHECK(ptr);
        CHECK(ptr.GetID() > 0);
        ptr.Delete();
    }

    SECTION("Nullptr Construction") {
        cse::Aptr<int> ptr(nullptr);
        CHECK(!ptr);
        ptr.Delete();
    }
}

TEST_CASE("Aptr Copy Semantics", "[Aptr][Copy]") 
{
    SECTION("Copy Construction") {
        auto original = cse::MakeAudited<int>(100);
        cse::Aptr<int> copy(original);
        
        CHECK(*original == *copy); // Same value
        original.Delete();
        copy.Delete();
    }

    SECTION("Copy Assignment") {
        auto a = cse::MakeAudited<int>(10);
        auto b = cse::MakeAudited<int>(20);
        a = b;
        
        CHECK(*a == *b); // Same value
        a.Delete();
        b.Delete();
    }

    SECTION("Self Assignment") {
        auto ptr = cse::MakeAudited<int>(30);
        auto original_id = ptr.GetID();
        ptr = ptr;
        
        CHECK(*ptr == 30);
        CHECK(ptr.GetID() == original_id); // ID remains same
        ptr.Delete();
    }
}

TEST_CASE("Aptr Move Semantics", "[Aptr][Move]") 
{
    SECTION("Move Construction") {
        auto original = cse::MakeAudited<int>(200);
        auto original_id = original.GetID();
        
        cse::Aptr<int> moved(std::move(original));
        
        CHECK(moved.GetID() == original_id);
        CHECK(original.GetID() == 0); // NOLINT
        original.Delete();
        moved.Delete();
    }

    SECTION("Move Assignment") {
        auto a = cse::MakeAudited<int>(40);
        auto b = cse::MakeAudited<int>(50);
        
        a = std::move(b);
        
        CHECK(b.GetID() == 0); // NOLINT
        a.Delete();
        b.Delete();
    }
}

TEST_CASE("Aptr Memory Management", "[Aptr][Memory]") 
{
    SECTION("Explicit Deletion") {
        auto ptr = cse::MakeAudited<std::string>("test");
        auto id = ptr.GetID();
        
        ptr.Delete();
        
        CHECK(cse::Aptr<std::string>::Find(id) == std::nullopt);
    }


    SECTION("Multiple References") {
        auto a = cse::MakeAudited<int>(100);
        auto b = a; // Copy
        auto c = b; // Copy
        
        CHECK(*a == 100);
        CHECK(*b == 100);
        CHECK(*c == 100);
        
        a.Delete();
        b.Delete();
        c.Delete();
    }
}

TEST_CASE("Aptr Advanced Operations", "[Aptr][Advanced]") 
{
    // SECTION("ID Uniqueness") {
    //     std::set<int> ids;
    //     for (int i = 0; i < 100; ++i) {
    //         auto ptr = cse::MakeAudited<int>(i);
    //         ids.insert(ptr.GetID());
    //         ptr.Delete();
    //     }
    //     CHECK(ids.size() == 100); // All IDs unique
    // }

    SECTION("Active Tracking") {
        cse::Aptr<int>::Reset();
        
        auto a = cse::MakeAudited<int>(1);
        auto b = cse::MakeAudited<int>(2);
        auto c = cse::MakeAudited<int>(3);
        
        auto active = cse::Aptr<int>::GetActiveAptrs();
        CHECK(active.size() == 3);

        a.Delete();
        b.Delete();
        c.Delete();
    }

    SECTION("Find Operation") {
        auto ptr = cse::MakeAudited<std::string>("find_me");
        auto id = ptr.GetID();
        auto found = cse::Aptr<std::string>::Find(id);
        
        ptr.Delete();
    }

    SECTION("Reset Operation") {
        auto a = cse::MakeAudited<int>(1);
        auto b = cse::MakeAudited<int>(2);
        
        cse::Aptr<int>::Reset();
        auto active = cse::Aptr<int>::GetActiveAptrs();
        CHECK(active.empty());
        a.Delete();
        b.Delete();
    }
}

// TEST_CASE("Aptr Leak Detection", "[Aptr][Leak]") 
// {
//     // This will trigger the LeakChecker's termination
//     // Should be run as a separate test case
//     auto ptr = cse::MakeAudited<int>(42);
//     // ptr goes out of scope without Delete()
//     // LeakChecker should catch this and terminate
// }