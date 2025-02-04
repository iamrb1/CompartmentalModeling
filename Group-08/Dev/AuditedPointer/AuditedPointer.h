#pragma once

#include <iostream>
#include <unordered_set>
#include <memory>

#ifdef DEBUG
#define DEBUG_MODE true
#else
#define DEBUG_MODE false
#endif

template <typename T>
class AuditedPointer
{
public:
    // Constructor
    explicit AuditedPointer(T* ptr = nullptr);

    // Destructor
    ~AuditedPointer();

    // Copy constructor
    AuditedPointer(const AuditedPointer& other);

    // Copy assignment operator
    AuditedPointer& operator=(const AuditedPointer& other);

    // Move constructor
    AuditedPointer(AuditedPointer&& other) noexcept;

    // Move assignment operator
    AuditedPointer& operator=(AuditedPointer&& other) noexcept;

    // Dereference operator
    T& operator*() const;

    // Arrow operator
    T* operator->() const;

    // Get the raw pointer
    T* get() const;

    // Check if the pointer is null
    explicit operator bool() const;

    // Delete the managed object
    void Delete();

    // Function to check for leaks
    static void CheckForLeaks();

private:
    T* ptr_;
    static std::unordered_set<T*> trackedPointers_;
};

// Stand-alone MakeAudited function
template <typename T, typename... Args>
AuditedPointer<T> MakeAudited(Args&&... args);

// Global object to check for leaks at program exit
struct LeakChecker
{
    ~LeakChecker();
};

extern LeakChecker leakChecker;