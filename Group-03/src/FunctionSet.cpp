/**
 * @file FunctionSet.cpp
 * @author jiang
 */

#include "FunctionSet.h"

#include <iostream>
#include <string>
#include <cassert>

namespace cse {

/**
 * Constructor
 */
template <typename ReturnType, typename... FunctionParams>
cse::FunctionSet<ReturnType, FunctionParams...>::FunctionSet() = default;

/**
 * Destructor
 */
template <typename ReturnType, typename... FunctionParams>
cse::FunctionSet<ReturnType, FunctionParams...>::~FunctionSet() = default;

/**
 * addFunction
 */
template <typename ReturnType, typename... FunctionParams>
void cse::FunctionSet<ReturnType, FunctionParams...>::AddFunction(const FunctionType& func)
{
    // Check if the function object is valid.
    if (func == nullptr) {
        throw std::invalid_argument("AddFunction: Function target is null.");
    }

    using PointerType = ReturnType(*)(FunctionParams...); // chatgpt

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

/**
 * Find the index of the function
 */
template <typename ReturnType, typename... FunctionParams>
int cse::FunctionSet<ReturnType, FunctionParams...>::FindFunctionIndex(const FunctionType& func) const {
    if (func == nullptr) {
        throw std::invalid_argument("FindFunctionIndex: Function target is null.");
    }

    using PointerType = ReturnType(*)(FunctionParams...);

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
template <typename ReturnType, typename... FunctionParams>
bool cse::FunctionSet<ReturnType, FunctionParams...>::RemoveFunction(const FunctionType& func)
{
    if (func == nullptr)
    {
        throw std::invalid_argument("RemoveFunction: Function target is null.");
    }
    int index = FindFunctionIndex(func);
    if (index >= 0)
    {
        mFunctions.erase(mFunctions.begin() + index);
        return true;
    }
    return false;
}

// Explicit template instantiations
template class cse::FunctionSet<int, int>;
template class cse::FunctionSet<double, double>;
template class cse::FunctionSet<void, const std::string&>;

}
