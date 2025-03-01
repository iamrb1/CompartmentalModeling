#include "../../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../AuditedPointer/AuditedPointer.h"

TEST_CASE("Test Aptr advanced operations", "[Aptr]") 
{
    // Test ID assignment and retrieval
    Aptr<int> aptr1 = MakeAudited<int>(10);
    Aptr<int> aptr2 = MakeAudited<int>(20);

    REQUIRE(aptr1.GetID() != aptr2.GetID()); // IDs should be unique
    REQUIRE(aptr1.GetID() > 0); // IDs should be positive
    REQUIRE(aptr2.GetID() > 0);

    // Test active Aptr tracking
    auto activeAptrs = Aptr<int>::GetActiveAptrs();
    REQUIRE(activeAptrs.size() == 2); // Both pointers should be active
    REQUIRE(activeAptrs.find(aptr1.GetID()) != activeAptrs.end());
    REQUIRE(activeAptrs.find(aptr2.GetID()) != activeAptrs.end());

    // Test deletion and removal from active set
    int id = aptr1.GetID();
    aptr1.Delete(); // Delete the Aptr
    activeAptrs = Aptr<int>::GetActiveAptrs();
    REQUIRE(activeAptrs.find(id) == activeAptrs.end()); // Should no longer be in the active set

    // Test Reset function
    Aptr<int>::Reset(); // Reset all active Aptrs
    activeAptrs = Aptr<int>::GetActiveAptrs();
    REQUIRE(activeAptrs.empty()); // Active set should be empty after reset

    aptr2.Delete();
    aptr1.Delete();
}
