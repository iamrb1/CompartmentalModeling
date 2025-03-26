/**
 * @file FunctionSet.cpp
 * @author jiang
 */

#include "FunctionSet.h"

#include <iostream>
#include <string>
#include <cassert>

/**
 * Constructor
 */
template <typename R, typename... Args>
cse::FunctionSet<R, Args...>::FunctionSet() = default;

/**
 * Destructor
 */
template <typename R, typename... Args>
cse::FunctionSet<R, Args...>::~FunctionSet() = default;

/**
 * addFunction
 */
template <typename R, typename... Args>
void cse::FunctionSet<R, Args...>::AddFunction(const FunctionType& func)
{
    // Check if the function object is valid.
    if (func == nullptr)
{
    throw std::invalid_argument("AddFunction: Function target is null.");
}

    using PointerType = R(*)(Args...);

    if (func.target_type() == typeid(PointerType)) {
        const PointerType* fp = func.template target<PointerType>();
        // Ensure the target pointer is valid.
        if (fp == nullptr)
		{
    		throw std::invalid_argument("AddFunction: Function target is null.");
		}
    }

    mFunctions.push_back(func);
}




///**
// * clearAll
// */
//template <typename R, typename... Args>
//void cse::FunctionSet<R, Args...>::ClearAll()
//{
//
//    mFunctions.clear();
//}

/**
 * isEmpty
 */
//template <typename R, typename... Args>
//bool cse::FunctionSet<R, Args...>::IsEmpty() const
//{
//    return mFunctions.empty();
//}

/**
 * countFunSet
 */
//template <typename R, typename... Args>
//std::size_t cse::FunctionSet<R, Args...>::CountFun() const
//{
//    return mFunctions.size();
//}

/**
 * Find the index of the function
 */
template <typename R, typename... Args>
int cse::FunctionSet<R, Args...>::FindFunctionIndex(const FunctionType& func) const
{
        if (func == nullptr)
{
    throw std::invalid_argument("AddFunction: Function target is null.");
}
    using PointerType = R(*)(Args...);

    assert(func.target_type() == typeid(PointerType));

    const PointerType* funcPtr = func.template target<PointerType>();
    if (!funcPtr) return -1;

    for (int i = 0; i < static_cast<int>(mFunctions.size()); ++i)
    {
        const auto& storedFunc = mFunctions[i];
        if (storedFunc.target_type() == typeid(PointerType))
        {
            const PointerType* storedPtr = storedFunc.template target<PointerType>();
            if (storedPtr && *storedPtr == *funcPtr)
            {
                return i;
            }
        }
    }
    return -1;
}



/**
 * removeFunction
 */
template <typename R, typename... Args>
bool cse::FunctionSet<R, Args...>::RemoveFunction( const FunctionType& func)
{
        if (func == nullptr)
{
    throw std::invalid_argument("AddFunction: Function target is null.");
}
    int index = FindFunctionIndex(func);
    if (index >= 0)
    {
        mFunctions.erase(mFunctions.begin() + index);
        return true;
    }
    return false;
}
template class cse::FunctionSet<int, int>;                  /// For "FunctionSet<int,int>"
template class cse::FunctionSet<double, double>;
template class cse::FunctionSet<void, const std::string&>;  /// For "FunctionSet<void, const std::string&>"
