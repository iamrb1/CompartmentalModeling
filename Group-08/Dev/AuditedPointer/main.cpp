#include "AuditedPointer.h"

int main()
{
    // Example 1: No memory leak (properly deleted pointer)
    {
        AuditedPointer<int> ptr1 = MakeAudited<int>(42);
        std::cout << "ptr1 value: " << *ptr1 << std::endl;
        ptr1.Delete(); // Properly delete the pointer
    }

    // Example 2: Memory leak (pointer not deleted)
    {
        AuditedPointer<int> ptr2 = MakeAudited<int>(100);
        std::cout << "ptr2 value: " << *ptr2 << std::endl;
        // ptr2.Delete(); // Intentionally not deleting to cause a memory leak
    }

    // Example 3: Another memory leak (pointer not deleted)
    {
        AuditedPointer<int> ptr3 = MakeAudited<int>(200);
        std::cout << "ptr3 value: " << *ptr3 << std::endl;
        // ptr3.Delete(); // Intentionally not deleting to cause a memory leak
    }

    // The leakChecker will automatically check for leaks when the program exits
    return 0;
}