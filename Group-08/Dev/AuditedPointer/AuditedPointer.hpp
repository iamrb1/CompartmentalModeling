#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <set>
#include <utility>
#include <type_traits>
#include <cassert>  

namespace cse {

#ifdef NDEBUG

// ==================== Non-Debug Mode (Raw Pointer) ====================

template <typename TYPE>
class Aptr
{
public:
    // Constructor
    explicit constexpr Aptr(TYPE* ptr = nullptr) : mPtr(ptr) {}

    // Copy constructor
    Aptr(const Aptr& other) : mPtr(new TYPE(*other.mPtr)) {}

    // Copy assignment operator
    Aptr& operator=(const Aptr& other)
    {
        if (this != &other) {
            mPtr = new TYPE(*other.mPtr);
        }
        return *this;
    }

    // Move constructor
    Aptr(Aptr&& other) noexcept : mPtr(other.mPtr)
    {
        other.mPtr = nullptr;
    }

    // Move assignment operator
    Aptr& operator=(Aptr&& other) noexcept
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
    explicit constexpr operator bool() const { return mPtr != nullptr; }

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
    auto lambda_new = [](auto&&... packed) {
        return new TYPE(std::forward<decltype(packed)>(packed)...);
    };
    return Aptr<TYPE>(lambda_new(std::forward<Args>(args)...));
}

#else

// ==================== Debug Mode (Audited Pointer) ====================

template <typename TYPE>
class Aptr;

struct LeakChecker
{
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

    void AddAptr(int id, const void* ptr)
    {
        activeAptrs[id] = ptr;
    }

    void RemoveAptr(int id)
    {
        activeAptrs.erase(id);
    }

    std::unordered_map<int, const void*> GetActiveAptrs() const
    {
        return activeAptrs;
    }

    const void* FindAptr(int id) const
    {
        auto it = activeAptrs.find(id);
        if (it != activeAptrs.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void Reset()
    {
        activeAptrs.clear();
    }
};

static LeakChecker leakChecker;


template <typename TYPE>
class Aptr
{
public:
    // Constructor
    explicit constexpr Aptr(TYPE* ptr = nullptr) : mPtr(ptr), mID(GetNextID())
    {
        if (mPtr)
        {
            leakChecker.AddAptr(mID, mPtr);
        }
    }

    // Destructor to catch non-deleted Aptrs at runtime
    ~Aptr() noexcept
    {
        if (mPtr) {
            leakChecker.RemoveAptr(mID);
            assert(false && "Aptr destroyed without calling Delete() - memory leak!");
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
            mPtr = other.mPtr ? new TYPE(*other.mPtr) : nullptr;
            if (mPtr)
            {
                leakChecker.AddAptr(mID, mPtr);
            }
        }
        return *this;
    }

    // Move constructor
    Aptr(Aptr&& other) noexcept : mPtr(other.mPtr), mID(other.mID)
    {
        other.mPtr = nullptr;
        other.mID = 0;
    }

    // Move assignment operator
    Aptr& operator=(Aptr&& other) noexcept
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
        assert(mPtr && "Dereferencing a null pointer");
        return *mPtr;
    }

    // Arrow operator
    TYPE* operator->() const
    {
        assert(mPtr && "Accessing a null pointer");
        return mPtr;
    }

    // Check if the pointer is null
    explicit constexpr operator bool() const
    {
        return mPtr;
    }

    // Delete the managed object
    void Delete()
    {
        if (!mPtr)
        {
            return;
        }
        leakChecker.RemoveAptr(mID);
        delete mPtr;
        mPtr = nullptr;
    }

    // Get the ID of this AuditedPointer
    constexpr int GetID() const
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
    TYPE* mPtr;
    int mID;

    static constexpr int StartID = 1;

    static int GetNextID()
    {
        static int nextID = StartID;
        return nextID++;
    }
};

// Stand-alone MakeAudited function
template <typename TYPE, typename... Args>
Aptr<TYPE> MakeAudited(Args&&... args)
{
    auto lambda_new = [](auto&&... packed) {
        // Lambda function takes the declared type of the provided value if possible
        return new TYPE(std::forward<decltype(packed)>(packed)...);
    };
    return Aptr<TYPE>(lambda_new(std::forward<Args>(args)...));
}

#endif

}
