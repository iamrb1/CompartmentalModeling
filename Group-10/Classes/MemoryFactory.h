/* --- MemoryFactory.h --- */

/* ------------------------------------------
Author: Devon FoxElster
Date: 1/31/2025
------------------------------------------ */

#include <list>
#include <set>

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

  /// @brief Default size of the MemoryFactory
  int allocationSize = 10;

  /// @brief List containing all allocated objects
  std::list<Object*> allocatedBlock;

  /// @brief Points to the next available Object
  typename std::list<Object*>::iterator reservedPoint = allocatedBlock.begin();

  /// @brief Int indicating how many free objects are left
  int freeSpace = allocationSize;

 public:
  /**
   * @brief MemoryFactory Basic Constructor
   */
  MemoryFactory() {
    for (int i = 0; i <= allocationSize; i++) {
      Object newObject = new Object;
      allocatedBlock.push_back(newObject);
    }
  }

  MemoryFactory(int newAllocSize) {
    assert(newAllocSize > 0);
    allocationSize = newAllocSize;
    for (int i = 0; i <= allocationSize; i++) {
      Object* newObject = new Object;
      allocatedBlock.push_back(newObject);
    }
  }

  /**
   * @brief MemoryFactory Basic Destructor
   */
  ~MemoryFactory() {
    for (auto object : allocatedBlock) {
      delete object;
    }
  }

  /**
   * @brief Returns a pointer to a reserved Object for the user
   */
  Object* Allocate() {
    /* TODO: Check if all space is taken by checking freeSpace before returning
     * a pointer. If so, return exception
     */
    Object* currentChoice = *reservedPoint;
    reservedPoint++;
    freeSpace--;
    return currentChoice;
  }

  void Deallocate(Object* targetObject) {
    assert(targetObject != nullptr);
    /* Rough Idea: Save the object pointer, iterate over list to find it's
     * location Erase the object dereference the object pointer and append it to
     * the list if the iterator points at end() before you append, make sure it
     * is set to the new element
     * Finally, increment freeSpace */
    for (Object& object : allocatedBlock) {
      if (&object == targetObject) {
        allocatedBlock.erase(object);
        allocatedBlock.push_back(object);
        break;
      }
    }
  }

  int GetSpace() { return freeSpace; }
};
}
#endif  // MEMORYFACTORY_H
