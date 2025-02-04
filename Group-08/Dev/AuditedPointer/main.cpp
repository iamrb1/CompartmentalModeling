#include "AuditedPointer.h"

int main()
{
    // Example usage
    AuditedPointer<int> ptr = MakeAudited<int>(42);
    std::cout << *ptr << std::endl; // Output: 42
    ptr.Delete();

    // Uncommenting the following lines will cause the program to terminate with an error
    // AuditedPointer<int> ptr2 = MakeAudited<int>(100);
    // ptr2.Delete();
    // *ptr2; // This will cause an error in DEBUG mode

    return 0;
}