/**
 * @file FunctionSet.h
 * @author jiang
 */

#ifndef FUNCTIONSET_H
#define FUNCTIONSET_H

#include <vector>
#include <functional>
#include <type_traits>
#include <stdexcept>

/**
 * @brief A container that holds multiple callable objects sharing the same signature
 *        and can invoke them all using a single call.
 *
 * @param ReturnType    The return type of the functions.
 * @param FunctionParams The argument types of the functions.
 */
namespace cse
{
    template <typename ReturnType, typename... FunctionParams>
    class FunctionSet
    {
    public:
        using FunctionType = std::function<ReturnType(FunctionParams...)>; // chatgpt

        /// Constructor
        FunctionSet();

        /// Destructor
        ~FunctionSet();

        /**
         * @brief Add a function to the set
         * @param func The actual function to store
         */
        void AddFunction(const FunctionType& func);

        /**
         * @brief Clear all the functions in the function set
         */
        void ClearAll() { mFunctions.clear(); }

        /**
         * @brief Check if the set is empty
         * @return true if empty; false otherwise
         */
        bool IsEmpty() const { return mFunctions.empty(); }

        /**
         * @brief Return the size of the function set
         * @return number of functions stored
         */
        std::size_t CountFun() const { return mFunctions.size(); };

        /**
         * @brief Return the index number of the function and use it for later. (This only works for real function pointers or non-capturing lambdas )
         * @param func the actual function to find
         * @return an int that tells the position of the function
         */
        int FindFunctionIndex(const FunctionType& func) const;

        /**
         * @brief Remove a function from the set (by reference).
         *        Real usage may require a different approach, because std::function
         *        doesn't have operator== for comparing contents.
         * @param func the actual function to remove
         * @return true if the function was found and removed, false otherwise
         */
        bool RemoveFunction(const FunctionType& func);

        /**
         * @brief Call all functions in the set with the given arguments
         * @param parameters The parameters to pass to each function
         * @return Depending on ReturnType, returns either std::vector<ReturnType> or void
         */
        auto CallAll(FunctionParams... parameters)
        {
            return callAllImpl(std::is_void<ReturnType>{}, parameters...); // chatgpt
        }

    private:
        /// Storage for functions    
        std::vector<FunctionType> mFunctions;

        /**
         * @brief Implementation for CallAll when ReturnType is not void
         * @param parameters Parameters to pass to each function
         * @return Vector containing the results of each function call
         */
        template <typename... ParamTypes>
        std::vector<ReturnType> callAllImpl(std::false_type /* isVoid */, ParamTypes... parameters)
        {
            std::vector<ReturnType> results;
            results.reserve(mFunctions.size());
            for (auto &func : mFunctions)
            {
                results.push_back(func(parameters...));
            }
            return results;
        }

        /**
         * @brief Implementation for CallAll when ReturnType is void
         * @param parameters Parameters to pass to each function
         */
        template <typename... ParamTypes>
        void callAllImpl(std::true_type /* isVoid */, ParamTypes... parameters)
        {
            for (auto &func : mFunctions)
            {
                func(parameters...);
            }
        }
    };
}

#endif // FUNCTIONSET_H
