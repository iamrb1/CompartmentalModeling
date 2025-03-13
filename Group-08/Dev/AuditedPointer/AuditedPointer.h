#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <set>

namespace cse {

#ifdef NDEBUG

// ==================== Non-Debug Mode (Raw Pointer) ====================

template <typename TYPE>
class Aptr
{
public:
    // Constructor
    explicit Aptr(TYPE* ptr = nullptr) : mPtr(ptr) {}

    // Copy constructor
    Aptr(const Aptr& other) : mPtr(new TYPE(*other.mPtr)) {}

    // Copy assignment operator
    Aptr& operator=(const Aptr& other)
    {
        if (this != &other) {
            delete mPtr;
            mPtr = new TYPE(*other.mPtr);
        }
        return *this;
    }

    // Move constructor
    Aptr(Aptr&& other) : mPtr(other.mPtr)
    {
        other.mPtr = nullptr;
    }

    // Move assignment operator
    Aptr& operator=(Aptr&& other)
    {
        if (this != &other) {
            delete mPtr;
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        return *this;
    }

    // Dereference operator
    TYPE& operator*() const { return *mPtr; }

    // Arrow operator
    TYPE* operator->() const { return mPtr; }

    // Check if the pointer is null
    explicit operator bool() const { return mPtr != nullptr; }

    // Delete the managed object
    void Delete()
    {
        delete mPtr;
        mPtr = nullptr;
    }

private:
    TYPE* mPtr;
};

// Stand-alone MakeAudited function
template <typename TYPE, typename... Args>
Aptr<TYPE> MakeAudited(Args&&... args)
{
    return Aptr<TYPE>(new TYPE(std::forward<Args>(args)...));
}

#else

// ==================== Debug Mode (Audited Pointer) ====================

template <typename TYPE>
class Aptr;

// LeakChecker struct to track active Aptr objects
struct LeakChecker
{
    // Map to store active Aptr objects (ID -> pointer)
    std::unordered_map<int, const void*> activeAptrs;

    ~LeakChecker()
    {
        if (!activeAptrs.empty())
        {
            std::cerr << "Memory leak detected: " << activeAptrs.size() << " objects not deleted." << std::endl;
            for (const auto& pair : activeAptrs)
            {
                std::cerr << "Leaked object ID: " << pair.first << std::endl;
            }
            std::terminate();
        }
    }

    // Add an Aptr to the active set
    void AddAptr(int id, const void* ptr)
    {
        activeAptrs[id] = ptr;
    }

    // Remove an Aptr from the active set
    void RemoveAptr(int id)
    {
        activeAptrs.erase(id);
    }

    // Get the active Aptr map
    std::unordered_map<int, const void*> GetActiveAptrs() const
    {
        return activeAptrs;
    }

    // Find an Aptr by ID
    const void* FindAptr(int id) const
    {
        auto it = activeAptrs.find(id);
        if (it != activeAptrs.end())
        {
            return it->second;
        }
        return nullptr;
    }

    // Reset all active Aptr objects
    void Reset()
    {
        activeAptrs.clear();
    }
};

// Global instance of LeakChecker
static LeakChecker leakChecker;

template <typename TYPE>
class Aptr
{
public:
    // Constructor
    explicit Aptr(TYPE* ptr = nullptr) : mPtr(ptr), mID(GetNextID())
    {
        if (mPtr)
        {
            leakChecker.AddAptr(mID, mPtr);
        }
    }

    // Copy constructor
    Aptr(const Aptr& other) : mPtr(other.mPtr ? new TYPE(*other.mPtr) : nullptr), mID(GetNextID())
    {
        if (mPtr)
        {
            leakChecker.AddAptr(mID, mPtr);
        }
    }

    // Copy assignment operator
    Aptr& operator=(const Aptr& other)
    {
        if (this != &other)
        {
            if (mPtr)
            {
                leakChecker.RemoveAptr(mID);
            }
            delete mPtr;
            mPtr = other.mPtr ? new TYPE(*other.mPtr) : nullptr;
            if (mPtr)
            {
                leakChecker.AddAptr(mID, mPtr);
            }
        }
        return *this;
    }

    // Move constructor
    Aptr(Aptr&& other) : mPtr(other.mPtr), mID(other.mID)
    {
        other.mPtr = nullptr;
        other.mID = 0;
    }

    // Move assignment operator
    Aptr& operator=(Aptr&& other)
    {
        if (this != &other)
        {
            if (mPtr)
            {
                leakChecker.RemoveAptr(mID);
            }
            delete mPtr;
            mPtr = other.mPtr;
            mID = other.mID;
            other.mPtr = nullptr;
            other.mID = 0;
        }
        return *this;
    }

    // Dereference operator
    TYPE& operator*() const
    {
        if (!mPtr)
        {
            std::cerr << "Dereferencing a null pointer (ID: " << mID << ")" << std::endl;
            std::terminate();
        }
        return *mPtr;
    }

    // Arrow operator
    TYPE* operator->() const
    {
        if (!mPtr)
        {
            std::cerr << "Accessing a null pointer (ID: " << mID << ")" << std::endl;
            std::terminate();
        }
        return mPtr;
    }

    // Check if the pointer is null
    explicit operator bool() const
    {
        return mPtr != nullptr;
    }

    // Delete the managed object
    void Delete()
    {
        if (!mPtr)
        {
            std::cerr << "Deleting a null pointer (ID: " << mID << ")" << std::endl;
            std::terminate();
        }
        leakChecker.RemoveAptr(mID);
        delete mPtr;
        mPtr = nullptr;
    }

    // Get the ID of this AuditedPointer
    int GetID() const
    {
        return mID;
    }

    // Get a set of all active AuditedPointer IDs and their pointers
    static std::unordered_map<int, const void*> GetActiveAptrs()
    {
        return leakChecker.GetActiveAptrs();
    }
    
    // Find an AuditedPointer by ID in the active set
    static const void* Find(int id)
    {
        return leakChecker.FindAptr(id);
    }
    
    // Reset all active AuditedPointers (delete and clear the active set)
    static void Reset()
    {
        leakChecker.Reset();
    }

private:
    // Base raw pointer
    TYPE* mPtr;

    // Unique ID for this AuditedPointer
    int mID;

    // Static counter for generating unique IDs
    static int GetNextID()
    {
        static int nextID = 1;
        return nextID++;
    }
};

// Stand-alone MakeAudited function
template <typename TYPE, typename... Args>
Aptr<TYPE> MakeAudited(Args&&... args)
{
    TYPE* ptr = new TYPE(std::forward<Args>(args)...);
    return Aptr<TYPE>(ptr);
}

#endif

} 