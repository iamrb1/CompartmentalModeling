#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace cse {
    class DataFileManager {
    private:
        std::string filePath;
        std::string initialFileInfo;

        // Easiest way to store functions is through an unordered map
        // https://stackoverflow.com/questions/55520876/creating-an-unordered-map-of-stdfunctions-with-any-arguments
        std::unordered_map<std::string, std::function<void()>> functionMap;

        bool updateFlag = false;
    public:
        // Constructors
        DataFileManager() = default;
        ~DataFileManager() = default;

        void openFile(const std::string& path);
        void update();
        void addFunction(const std::string& name, const std::function<void()>& lambda);

        // Delete a function from our map
        void deleteFunction(const std::string& name){
            // https://cplusplus.com/reference/unordered_map/unordered_map/erase/
            auto it = functionMap.find(name);
            if (it != functionMap.end()) {
                functionMap.erase(it);
            }
        }

        // List each function we have stored so far
        void listFunctions(){
            // https://stackoverflow.com/questions/22880431/iterate-through-unordered-map-c
            for (const auto & [ key, value ] : functionMap) {
                std::cout << key << std::endl;
            }
        }
        // Clear every function
        void clearFunctions(){
            functionMap.clear();
        }

        void closeFile();

    };
}
#endif // DATAFILEMANAGER_H
