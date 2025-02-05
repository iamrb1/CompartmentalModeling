#include "AuditedPointer.h"

int main()
{
    // Example usage
    AuditedPointer<int> ptr = MakeAudited<int>(42);
    std::cout << *ptr << std::endl; // Output: 42
    ptr.Delete();

    // EXAMPLES OF ERRORS SHOWN IN DEBUG MODE
    // The following code, when uncommented, will
    // create errors when compiled with the -DDEBUG
    // flag.

    // EXAMPLE 1 - Dereferencing a deleted pointer
    // AuditedPointer<int> ptr2 = MakeAudited<int>(100);
    // ptr2.Delete();
    // *ptr2; 

    // EXAMPLE 2 - Deleting a deleted pointer
    // AuditedPointer<int> ptr3 = MakeAudited<int>(200);
    // ptr3.Delete();
    // ptr3.Delete();

    // EXAMPLE 3 - Not deleting all AuditedPointers by termination
    // AuditedPointer<const std::string> ptr4 = MakeAudited<const std::string>("Hello");

    return 0;
}