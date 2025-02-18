/* --- MemoryFactory.h --- */

/* ------------------------------------------
Author: Devon FoxElster
Date: 1/31/2025
------------------------------------------ */

#include <list>
#include <stdexcept>
#include <tuple>

#ifndef MEMORYFACTORY_H
#define MEMORYFACTORY_H
namespace cse {
/**
 * @brief A Class designed to allocate objects in bulk in order to speed up
 * processes
 * @tparam Object A templated object pointing to the factory type
 */
template <typename Object>
class MemoryFactory {
 private:
  /// @brief Default size of the MemoryFactory
  int allocationSize_ = 10;

  /// @brief List containing all allocated objects
  std::list<Object*> allocatedBlock_;

  /// @brief Points to the next available Object
  typename std::list<Object*>::iterator reservedPoint_;

  /// @brief Int indicating how many free objects are left
  int reservedObjects_ = 0;

  /// @brief Default Value for all Objects
  Object defaultObject_;

  /**
   * @brief Private Function to handle dynamic expansion of allocated objects
   *
   * @param allocationAmount how many objects to be allocated for future use
   * @param initialState Object instance that should be assigned to all new
   * objects
   *
   * AllocateObjects iterates over the allocation amount and reserves new
   * objects.
   *
   * Use Cases:
   *  Initial Construction - This creates all initial objects
   *  Expansion - This doubles the current size of the MemoryFactory
   */
  void AllocateObjects_(int allocationAmount, Object initialState = Object{}) {
    for (int i = 0; i < allocationAmount; i++) {
      Object* newObject = new Object{};

      /* If the desired state is not DEFAULT
       * then reassign the object to the desired value */
      *newObject = (*newObject != initialState) ? initialState : *newObject;
      allocatedBlock_.push_back(newObject);

      /** The function is only ever called when no objects exist in the
       * MemoryFactory or if all current objects are reserved, so the reserved
       * point should now point at the newest object to be added to the factory
       */
      if (i == 0) {
        reservedPoint_ = --allocatedBlock_.end();
      }
    }
  }

  /**
   * @brief Doubles the amount of allocated Objects within the MemoryFactory
   */
  void ExpandSpace_() {
    AllocateObjects_(allocationSize_, defaultObject_);
    allocationSize_ = allocationSize_ * 2;
  }

 public:
  /**
   * @brief MemoryFactory Basic Constructor
   *
   * @param newAllocSize New count of initially allocated objects
   * @param initialState Alternate starting value for all Objects in the factory
   */
  MemoryFactory(int newAllocSize = 10, Object initialState = Object{}) {
    // No point in a MemoryFactory that doesn't store objects
    assert(newAllocSize > 0);
    allocationSize_ = newAllocSize;
    AllocateObjects_(allocationSize_, initialState);
    defaultObject_ = initialState;
  }

  /**
   * @brief MemoryFactory Basic Destructor
   */
  ~MemoryFactory() {
    for (auto object : allocatedBlock_) {
      delete object;
    }
  }

  /**
   * @brief Returns a pointer to a reserved Object for the user
   *
   * @tparam newArgs Any arguments to pass to the Object's constructor
   * 
   * @return A pointer to the reserved Object
   */

  template <typename... newArgs>
  Object* Allocate(newArgs... assignedValues) {
    // MemoryFactory is out of space
    if (reservedPoint_ == allocatedBlock_.end()) {
      ExpandSpace_();
    }
    Object* allocatedObject = *reservedPoint_;
    reservedPoint_++;
    reservedObjects_++;

    // If no constructor arguments are passed use the default value
    if (sizeof...(assignedValues) != 0) {
      *allocatedObject = Object{assignedValues...};
    } else {
      *allocatedObject = defaultObject_;
    }
    return allocatedObject;
  }

  /**
   * @brief Returns an object to the MemoryFactory once it is done being used
   * @param targetObject Object pointer to the deallocated Object
   *
   * @details Will take the returned Object, reset it to the default state, and
   * add it back to the pool of available Objects
   */
  void Deallocate(Object* targetObject) {
    // This needs to point to an object
    assert(targetObject != nullptr);
    bool objectFound = false;

    for (auto iterator = allocatedBlock_.begin();
         iterator != allocatedBlock_.end(); iterator++) {
      if (*iterator == targetObject) {
        allocatedBlock_.erase(iterator);
        *targetObject = defaultObject_;
        allocatedBlock_.push_back(targetObject);
        objectFound = true;
        break;
      }
    }
    if (objectFound) {
      reservedObjects_--;
    } else {
      throw std::invalid_argument(
          "This object does not belong to this MemoryFactory");
    }
  }

  /**
   * @brief Returns number of remaining objects in the factory
   * 
   * @return The number of available Objects in the MemoryFactory
   */
  int GetSpace() const { return allocationSize_ - reservedObjects_; }

  /**
   * @brief Returns how many Objects are allocated by the factory
   * 
   * @return The total number of Objects in the MemoryFactory
   */
  int GetSize() const { return allocationSize_; }
};
}  // namespace cse
#endif  // MEMORYFACTORY_H
