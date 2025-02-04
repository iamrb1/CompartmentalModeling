#include "AuditedPointer.h"

// Initialize static member
template <typename T>
std::unordered_set<T*> AuditedPointer<T>::trackedPointers_;

// Constructor
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
template <typename T>
AuditedPointer<T>::AuditedPointer(const AuditedPointer& other) : ptr_(other.ptr_)
{
    if (DEBUG_MODE && ptr_)
    {
        trackedPointers_.insert(ptr_);
    }
}

// Copy assignment operator
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
template <typename T>
AuditedPointer<T>::AuditedPointer(AuditedPointer&& other) noexcept : ptr_(other.ptr_)
{
    other.ptr_ = nullptr;
}

// Move assignment operator
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

// Get the raw pointer
template <typename T>
T* AuditedPointer<T>::get() const
{
    return ptr_;
}

// Check if the pointer is null
template <typename T>
AuditedPointer<T>::operator bool() const
{
    return ptr_ != nullptr;
}

// Delete the managed object
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

// Static function to check for leaks
template <typename T>
void AuditedPointer<T>::CheckForLeaks()
{
    if (DEBUG_MODE && !trackedPointers_.empty())
    {
        std::cerr << "Memory leak detected: " << trackedPointers_.size() << " pointers not deleted." << std::endl;
        std::terminate();
    }
}

// Stand-alone MakeAudited function
template <typename T, typename... Args>
AuditedPointer<T> MakeAudited(Args&&... args)
{
    return AuditedPointer<T>(new T(std::forward<Args>(args)...));
}

// LeakChecker destructor
LeakChecker::~LeakChecker()
{
    AuditedPointer<int>::CheckForLeaks(); // Use int as a placeholder type
}

// Global leak checker instance
LeakChecker leakChecker;