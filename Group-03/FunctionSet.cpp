/**
 * @file FunctionSet.cpp
 * @author jiang
 */

#include "FunctionSet.h"
#include <utility>  // for std::forward
#include <iostream> // just in case you need it
#include <string>   // for std::string usage

//
// 1) Provide definitions for each templated member function.
//    * You MUST precede each with: template <typename R, typename... Args>
//    * Then: return_type FunctionSet<R, Args...>::functionName(...) { ... }
//

/**
 * Constructor
 */
template <typename R, typename... Args>
FunctionSet<R, Args...>::FunctionSet() = default;

/**
 * Destructor
 */
template <typename R, typename... Args>
FunctionSet<R, Args...>::~FunctionSet() = default;

/**
 * addFunction
 */
template <typename R, typename... Args>
void FunctionSet<R, Args...>::addFunction(const FunctionType& func)
{
    mFunctions.push_back(func);
}

/**
 * removeFunction
 */
template <typename R, typename... Args>
void FunctionSet<R, Args...>::removeFunction(const FunctionType& func)
{
}

/**
 * clearAll
 */
template <typename R, typename... Args>
void FunctionSet<R, Args...>::clearAll()
{
    mFunctions.clear();
}

/**
 * isEmpty
 */
template <typename R, typename... Args>
bool FunctionSet<R, Args...>::isEmpty() const
{
    return mFunctions.empty();
}

/**
 * countFunSet
 */
template <typename R, typename... Args>
std::size_t FunctionSet<R, Args...>::countFunSet() const
{
    return mFunctions.size();
}

template class FunctionSet<int, int>;                  // For "FunctionSet<int,int>"
template class FunctionSet<void, const std::string&>;  // For "FunctionSet<void, const std::string&>"
// Add more lines if you use more template argument sets...
