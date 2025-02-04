// AuditedPointer.h

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

// ==================== Template Implementations ====================

template <typename T>
std::unordered_set<T*> AuditedPointer<T>::trackedPointers_;

template <typename T>
AuditedPointer<T>::AuditedPointer(T* ptr) : ptr_(ptr)
{
    if (DEBUG_MODE && ptr_)
    {
        trackedPointers_.insert(ptr_);
    }
}

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

template <typename T>
AuditedPointer<T>::AuditedPointer(const AuditedPointer& other) : ptr_(other.ptr_)
{
    if (DEBUG_MODE && ptr_)
    {
        trackedPointers_.insert(ptr_);
    }
}

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

template <typename T>
AuditedPointer<T>::AuditedPointer(AuditedPointer&& other) noexcept : ptr_(other.ptr_)
{
    other.ptr_ = nullptr;
}

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

template <typename T>
T* AuditedPointer<T>::get() const
{
    return ptr_;
}

template <typename T>
AuditedPointer<T>::operator bool() const
{
    return ptr_ != nullptr;
}

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

template <typename T>
void AuditedPointer<T>::CheckForLeaks()
{
    if (DEBUG_MODE && !trackedPointers_.empty())
    {
        std::cerr << "Memory leak detected: " << trackedPointers_.size() << " pointers not deleted." << std::endl;
        std::terminate();
    }
}

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