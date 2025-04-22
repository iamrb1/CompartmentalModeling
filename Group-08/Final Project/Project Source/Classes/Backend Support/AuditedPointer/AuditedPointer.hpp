#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <set>
#include <utility>
#include <type_traits>
#include <cassert>  
#include <optional>

namespace cse {

#ifdef NDEBUG

/**
 * @class   Aptr
 * @file    Aptr.hpp  
 * @brief   A smart pointer class that provides basic ownership semantics in non-debug mode.
 * 
 * @tparam TYPE The type of object being managed by the pointer
 */
template <typename TYPE>
class Aptr
{
public:
    /**
     * @brief Constructs an Aptr managing the given raw pointer
     * @param ptr Raw pointer to manage (defaults to nullptr)
     */
    explicit constexpr Aptr(TYPE* ptr = nullptr) : mPtr(ptr) {}

    /**
     * @brief Copy constructor - creates a deep copy of the managed object
     * @param other Aptr to copy from
     */
    Aptr(const Aptr& other) : mPtr(new TYPE(*other.mPtr)) {}

    /**
     * @brief Copy assignment operator - creates a deep copy of the managed object
     * @param other Aptr to copy from
     * @return Reference to this Aptr
     */
    Aptr& operator=(const Aptr& other)
    {
        if (this != &other) {
            mPtr = new TYPE(*other.mPtr);
        }
        return *this;
    }

    /**
     * @brief Move constructor - transfers ownership from another Aptr
     * @param other Aptr to move from
     */
    Aptr(Aptr&& other) noexcept : mPtr(other.mPtr)
    {
        other.mPtr = nullptr;
    }

    /**
     * @brief Move assignment operator - transfers ownership from another Aptr
     * @param other Aptr to move from
     * @return Reference to this Aptr 
     */
    Aptr& operator=(Aptr&& other) noexcept
    {
        if (this != &other) {
            delete mPtr;
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        return *this;
    }

    /**
     * @brief Dereference operator - provides access to the managed object
     * @return Reference to the managed object
     */
    TYPE& operator*() const { return *mPtr; }

    /**
     * @brief Member access operator - provides access to the managed object's members
     * @return Raw pointer to the managed object
     */
    TYPE* operator->() const { return mPtr; }

    /**
     * @brief Checks if the Aptr is managing an object
     * @return true if managing an object, false otherwise
     */
    explicit constexpr operator bool() const { return mPtr != nullptr; }

    /**
     * @brief Deletes the managed object and resets the Aptr to null
     */
    void Delete()
    {
        delete mPtr;
        mPtr = nullptr;
    }

private:
    TYPE* mPtr; ///< Raw pointer to the managed object
};

/**
 * @brief Creates a new Aptr managing a newly constructed object
 * @tparam TYPE Type of object to create
 * @tparam Args Types of constructor arguments
 * @param args Arguments to forward to the object's constructor
 * @return Aptr managing the new object
 */
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

/**
 * @class   Aptr
 * @file    Aptr.hpp
 * @brief   Audited pointer class for debug mode that tracks all active pointers and detects memory leaks.
 *          Provides additional debugging features like leak detection and pointer validation
 * 
 * @tparam TYPE The type of object being managed by the pointer
 */
template <typename TYPE>
class Aptr;

/**
 * @class   LeakChecker
 * @brief   Internal utility class that tracks all active Aptr instances and detects memory leaks
 */
struct LeakChecker
{
    std::unordered_map<int, const void*> activeAptrs; ///< Map of active pointer IDs to their addresses

    /**
     * @brief Destructor that checks for and reports memory leaks
     */
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

    /**
     * @brief Adds a new Aptr to the tracking system
     * @param id Unique ID of the Aptr
     * @param ptr Raw pointer being managed
     */
    void AddAptr(int id, const void* ptr)
    {
        activeAptrs[id] = ptr;
    }

    /**
     * @brief Removes an Aptr from the tracking system
     * @param id Unique ID of the Aptr to remove
     */
    void RemoveAptr(int id)
    {
        activeAptrs.erase(id);
    }

    /**
     * @brief Gets all currently active Aptrs
     * @return Map of active pointer IDs to their addresses
     */
    std::unordered_map<int, const void*> GetActiveAptrs() const
    {
        return activeAptrs;
    }

    /**
     * @brief Finds an Aptr by its ID
     * @param id Unique ID to search for
     * @return The raw pointer being managed or nullptr if not found
     */
    std::optional<const void*> FindAptr(int id) const
    {
        auto it = activeAptrs.find(id);
        if (it != activeAptrs.end())
        {
            return it->second;
        }
        return std::nullopt;
    }

    /**
     * @brief Clears all active Aptr tracking
     */
    void Reset()
    {
        activeAptrs.clear();
    }
};

static LeakChecker leakChecker; ///< Global instance of the leak checker


template <typename TYPE>
class Aptr
{
public:
    /**
     * @brief Constructs an Aptr managing the given raw pointer
     * @param ptr Raw pointer to manage (defaults to nullptr)
     */
    explicit constexpr Aptr(TYPE* ptr = nullptr) : mPtr(ptr), mID(GetNextID())
    {
        if (mPtr)
        {
            leakChecker.AddAptr(mID, mPtr);
        }
    }

    /**
     * @brief Copy constructor - creates a deep copy of the managed object
     * @param other Aptr to copy from
     */
    Aptr(const Aptr& other) : mPtr(other.mPtr ? new TYPE(*other.mPtr) : nullptr), mID(GetNextID())
    {
        if (mPtr)
        {
            leakChecker.AddAptr(mID, mPtr);
        }
    }

    /**
     * @brief Copy assignment operator - creates a deep copy of the managed object
     * @param other Aptr to copy from
     * @return Reference to this Aptr
     */
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

    /**
     * @brief Move constructor - transfers ownership from another Aptr
     * @param other Aptr to move from
     */
    Aptr(Aptr&& other) noexcept : mPtr(other.mPtr), mID(other.mID)
    {
        other.mPtr = nullptr;
        other.mID = 0;
    }

    /**
     * @brief Move assignment operator - transfers ownership from another Aptr
     * @param other Aptr to move from
     * @return Reference to this Aptr
     */
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

    /**
     * @brief Dereference operator - provides access to the managed object with null check
     * @return Reference to the managed object
     * @throws Assertion failure if pointer is null
     */
    TYPE& operator*() const
    {
        assert(mPtr && "Dereferencing a null pointer");
        return *mPtr;
    }

    /**
     * @brief Member access operator - provides access to the managed object's members with null check
     * @return Raw pointer to the managed object
     * @throws Assertion failure if pointer is null
     */
    TYPE* operator->() const
    {
        assert(mPtr && "Accessing a null pointer");
        return mPtr;
    }

    /**
     * @brief Checks if the Aptr is managing an object
     * @return true if managing an object, false otherwise
     */
    explicit constexpr operator bool() const
    {
        return mPtr;
    }

    /**
     * @brief Properly deletes the managed object and cleans up tracking
     */
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

    /**
     * @brief Gets the unique ID of this Aptr
     * @return The Aptr's tracking ID
     */
    constexpr int GetID() const
    {
        return mID;
    }

    /**
     * @brief Gets all currently active Aptrs
     * @return Map of active pointer IDs to their addresses
     */
    static std::unordered_map<int, const void*> GetActiveAptrs()
    {
        return leakChecker.GetActiveAptrs();
    }

    /**
     * @brief Finds an Aptr by its ID
     * @param id Unique ID to search for
     * @return The raw pointer being managed or nullptr if not found
     */
    static std::optional<const void*> Find(int id)
    {
        return leakChecker.FindAptr(id);
    }

    /**
     * @brief Clears all active Aptr tracking
     */
    static void Reset()
    {
        leakChecker.Reset();
    }

private:
    TYPE* mPtr; ///< Raw pointer to the managed object
    int mID;    ///< Unique tracking ID

    static constexpr int StartID = 1; ///< Starting value for ID generation

    /**
     * @brief Generates the next unique ID for Aptr tracking
     * @return The next available ID
     */
    static int GetNextID()
    {
        static int nextID = StartID;
        return nextID++;
    }
};

/**
 * @brief Creates a new Aptr managing a newly constructed object with debug tracking
 * @tparam TYPE Type of object to create
 * @tparam Args Types of constructor arguments
 * @param args Arguments to forward to the object's constructor
 * @return Aptr managing the new object
 */
template <typename TYPE, typename... Args>
Aptr<TYPE> MakeAudited(Args&&... args)
{
    auto lambda_new = [](auto&&... packed) {
        return new TYPE(std::forward<decltype(packed)>(packed)...);
    };
    return Aptr<TYPE>(lambda_new(std::forward<Args>(args)...));
}

#endif

} 