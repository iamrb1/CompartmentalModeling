#include "../../../../third-party/Catch/single_include/catch2/catch.hpp"
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
        CHECK(original.GetID() != copy.GetID()); // Different IDs
        original.Delete();
        copy.Delete();
    }

    SECTION("Copy Assignment") {
        auto a = cse::MakeAudited<int>(10);
        auto b = cse::MakeAudited<int>(20);
        auto a_id = a.GetID();
        a = b;
        
        CHECK(*a == *b); // Same value
        CHECK(a.GetID() == a_id); // ID remains same
        CHECK(b.GetID() != a_id); // Different IDs
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
        CHECK(!original); // Should be null after move
        moved.Delete();
    }

    SECTION("Move Assignment") {
        auto a = cse::MakeAudited<int>(40);
        auto b = cse::MakeAudited<int>(50);
        auto a_id = a.GetID();
        
        a = std::move(b);
        
        CHECK(a.GetID() == a_id); // ID remains same
        CHECK(b.GetID() == 0); // NOLINT
        CHECK(!b); // Should be null after move
        a.Delete();
    }
}

TEST_CASE("Aptr Memory Management", "[Aptr][Memory]") 
{
    SECTION("Explicit Deletion") {
        auto ptr = cse::MakeAudited<std::string>("test");
        auto id = ptr.GetID();
        
        CHECK(cse::Aptr<std::string>::Find(id).has_value());
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
        CHECK(a.GetID() != b.GetID());
        CHECK(b.GetID() != c.GetID());
        
        a.Delete();
        b.Delete();
        c.Delete();
    }
}

TEST_CASE("Aptr Advanced Operations", "[Aptr][Advanced]") 
{
    SECTION("ID Uniqueness") {
        std::set<int> ids;
        for (int i = 0; i < 100; ++i) {
            auto ptr = cse::MakeAudited<int>(i);
            ids.insert(ptr.GetID());
            ptr.Delete();
        }
        CHECK(ids.size() == 100); // All IDs unique
    }

    SECTION("Active Tracking") {
        cse::Aptr<int>::Reset();
        
        auto a = cse::MakeAudited<int>(1);
        auto b = cse::MakeAudited<int>(2);
        auto c = cse::MakeAudited<int>(3);
        
        auto active = cse::Aptr<int>::GetActiveAptrs();
        CHECK(active.size() == 3);
        CHECK(active.count(a.GetID()) == 1);
        CHECK(active.count(b.GetID()) == 1);
        CHECK(active.count(c.GetID()) == 1);

        a.Delete();
        active = cse::Aptr<int>::GetActiveAptrs();
        CHECK(active.size() == 2);

        b.Delete();
        c.Delete();
    }

    SECTION("Find Operation") {
        cse::Aptr<std::string>::Reset();
        
        auto ptr1 = cse::MakeAudited<std::string>("first");
        auto ptr2 = cse::MakeAudited<std::string>("second");
        auto ptr3 = cse::MakeAudited<std::string>("third");
        
        auto found1 = cse::Aptr<std::string>::Find(ptr1.GetID());
        CHECK(found1.has_value());
        CHECK(*found1 == ptr1.operator->());
        
        auto found2 = cse::Aptr<std::string>::Find(ptr2.GetID());
        CHECK(found2.has_value());
        CHECK(*found2 == ptr2.operator->());
        
        ptr1.Delete();
        ptr2.Delete();
        ptr3.Delete();
    }

    SECTION("Reset Operation") {
        cse::Aptr<int>::Reset();
        
        auto a = cse::MakeAudited<int>(1);
        auto b = cse::MakeAudited<int>(2);
        
        auto active = cse::Aptr<int>::GetActiveAptrs();
        CHECK(active.size() == 2);
        
        cse::Aptr<int>::Reset();
        active = cse::Aptr<int>::GetActiveAptrs();
        CHECK(active.empty());
        
        // Objects still exist until deleted
        CHECK(*a == 1);
        CHECK(*b == 2);
        
        a.Delete();
        b.Delete();
    }
}