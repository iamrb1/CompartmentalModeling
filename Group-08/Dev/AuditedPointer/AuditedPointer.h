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

    // Check if the pointer is null
    explicit operator bool() const;

    // Delete the managed object
    void Delete();

    // Function to check for leaks
    static void CheckForLeaks();

private:
    // Base raw pointer
    T* ptr_;

    // Set of pointers in the program to check for leaks
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

LeakChecker leakChecker;

// ==================== Template Implementations ====================

// Allocates memory for the set of tracked pointers
template <typename T>
std::unordered_set<T*> AuditedPointer<T>::trackedPointers_;

// Constructor
// ex: AuditedPointer<Example> ptr(new Example()); 
//// Creates an AuditedPointer managing a new Example object
template <typename T>
AuditedPointer<T>::AuditedPointer(T* ptr) : ptr_(ptr)
{
    if (DEBUG_MODE && ptr_)
    {
        trackedPointers_.insert(ptr_);
    }
}

// Destructor
template <typename T>
AuditedPointer<T>::~AuditedPointer()
{
    if (DEBUG_MODE)
    {
        if (ptr_)
        {
            trackedPointers_.erase(ptr_);
        }
    }
    delete ptr_;
}

// Copy constructor
// ex: AuditedPointer<Example> ptr2(ptr1); 
//// Creates a copy of `ptr1` (both manage the same Example object)
template <typename T>
AuditedPointer<T>::AuditedPointer(const AuditedPointer& other) : ptr_(other.ptr_)
{
    if (DEBUG_MODE && ptr_)
    {
        trackedPointers_.insert(ptr_);
    }
}

// Copy assignment operator
// ex AuditedPointer<Example> ptr2 = ptr1; 
//// Copies `ptr1` into `ptr2` (both manage the same Example object)
template <typename T>
AuditedPointer<T>& AuditedPointer<T>::operator=(const AuditedPointer& other)
{
    if (this != &other)
    {
        if (DEBUG_MODE)
        {
            if (ptr_)
            {
                trackedPointers_.erase(ptr_);
            }
            if (other.ptr_)
            {
                trackedPointers_.insert(other.ptr_);
            }
        }
        delete ptr_;
        ptr_ = other.ptr_;
    }
    return *this;
}

// Move constructor
// ex: AuditedPointer<Example> ptr2(std::move(ptr1)); 
//// Moves ownership from `ptr1` to `ptr2` (ptr1 becomes null)
template <typename T>
AuditedPointer<T>::AuditedPointer(AuditedPointer&& other) noexcept : ptr_(other.ptr_)
{
    other.ptr_ = nullptr;
}

// Move assignment opoerator
// AuditedPointer<Example> ptr2 = std::move(ptr1); 
//// Moves ownership from `ptr1` to `ptr2` (ptr1 becomes null)
template <typename T>
AuditedPointer<T>& AuditedPointer<T>::operator=(AuditedPointer&& other) noexcept
{
    if (this != &other)
    {
        if (DEBUG_MODE && ptr_)
        {
            trackedPointers_.erase(ptr_);
        }
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }
    return *this;
}

// Dereference operator
// ex: Example obj = *ptr; 
//// Dereferences `ptr` to access the managed Example object
template <typename T>
T& AuditedPointer<T>::operator*() const
{
    if (DEBUG_MODE)
    {
        if (!ptr_ || trackedPointers_.find(ptr_) == trackedPointers_.end())
        {
            std::cerr << "Dereferencing a deleted or null pointer" << std::endl;
            std::terminate();
        }
    }
    return *ptr_;
}

// Arrow operator
// ex: ptr->doSomething(); 
//// Accesses a member function or variable of the managed Example object
template <typename T>
T* AuditedPointer<T>::operator->() const
{
    if (DEBUG_MODE)
    {
        if (!ptr_ || trackedPointers_.find(ptr_) == trackedPointers_.end())
        {
            std::cerr << "Accessing a deleted or null pointer" << std::endl;
            std::terminate();
        }
    }
    return ptr_;
}

// Checks if there is a ptr within the object
template <typename T>
AuditedPointer<T>::operator bool() const
{
    return ptr_ != nullptr;
}

// Member function to delete the object
template <typename T>
void AuditedPointer<T>::Delete()
{
    if (DEBUG_MODE)
    {
        if (!ptr_ || trackedPointers_.find(ptr_) == trackedPointers_.end())
        {
            std::cerr << "Deleting a deleted or null pointer" << std::endl;
            std::terminate();
        }
        trackedPointers_.erase(ptr_);
    }
    delete ptr_;
    ptr_ = nullptr;
}

// Function ran at runtime that checks in DEBUG mode if
// all instances of the class are deleted
template <typename T>
void AuditedPointer<T>::CheckForLeaks()
{
    if (DEBUG_MODE && !trackedPointers_.empty())
    {
        std::cerr << "Memory leak detected: " << trackedPointers_.size() << " pointers not deleted." << std::endl;
        std::terminate();
    }
}

// MakeAudited helper function to turn classes to AuditedPointers
template <typename T, typename... Args>
AuditedPointer<T> MakeAudited(Args&&... args)
{
    // std::forward ensures lvalues and rvalues and perserved
    return AuditedPointer<T>(new T(std::forward<Args>(args)...));
}

// LeakChecker destructor
LeakChecker::~LeakChecker()
{
    AuditedPointer<int>::CheckForLeaks(); // Use int as a placeholder type
}