#include "AuditedPointer.hpp"
#include <vector>

int main()
{
    // Example usage
    cse::Aptr<int> ptr = cse::MakeAudited<int>(42);
    std::cout << *ptr << std::endl; // Output: 42
    ptr.Delete();

    // EXAMPLES OF ERRORS SHOWN IN DEBUG MODE
    // The following code, when uncommented, will
    // create errors when compiled with the -DDEBUG
    // flag.

    // EXAMPLE 1 - Dereferencing a deleted pointer
    // cse::Aptr<int> ptr2 = cse::MakeAudited<int>(100);
    // ptr2.Delete();
    // *ptr2; 

    // EXAMPLE 3 - Not deleting all Aptrs by termination
    cse::Aptr<const std::string> ptr4 = cse::MakeAudited<const std::string>("Hello");
    cse::Aptr<float> ptr5 = cse::MakeAudited<float>(3.496);
    ptr4.Delete();

    return 0;
}