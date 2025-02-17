/**
 * @file FunctionSet.cpp
 * @author jiang
 */

#include "FunctionSet.h"

#include <iostream>
#include <string>


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
void cse::FunctionSet<R, Args...>::addFunction(const FunctionType& func)
{
    using PointerType = R(*)(Args...);

    if (func.target_type() == typeid(PointerType)) {
        const PointerType* fp = func.template target<PointerType>();
        if (fp) {
            std::cout << "addFunction storing: " << (void*)(*fp) << std::endl;
        }
    }

    mFunctions.push_back(func);
}



/**
 * clearAll
 */
template <typename R, typename... Args>
void cse::FunctionSet<R, Args...>::clearAll()
{
    mFunctions.clear();
}

/**
 * isEmpty
 */
template <typename R, typename... Args>
bool cse::FunctionSet<R, Args...>::isEmpty() const
{
    return mFunctions.empty();
}

/**
 * countFunSet
 */
template <typename R, typename... Args>
std::size_t cse::FunctionSet<R, Args...>::countFunSet() const
{
    return mFunctions.size();
}

/**
 * Find the index of the function
 */
template <typename R, typename... Args>
int cse::FunctionSet<R, Args...>::findFunctionIndex(const FunctionType& func) const
{
    using PointerType = R(*)(Args...);

    if (func.target_type() == typeid(PointerType))
    {
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
    }
    return -1;
}



/**
 * removeFunction
 */
template <typename R, typename... Args>
void cse::FunctionSet<R, Args...>::removeFunction( const FunctionType& func)
{
    int index = findFunctionIndex(func);
    if (index >= 0)
    {
        mFunctions.erase(mFunctions.begin() + index);
    }
}
template class cse::FunctionSet<int, int>;                  // For "FunctionSet<int,int>"
template class cse::FunctionSet<void, const std::string&>;  // For "FunctionSet<void, const std::string&>"
// Add more lines if you use more template argument sets...
