#ifndef DATAFILEMANAGER_H
#define DATAFILEMANAGER_H

#include <string>

class DataFileManager {
public:
    DataFileManager(const std::string& path); // Constructor
    void update();
    void addFunction();
    void openFile();
    void closeFile();
    void clearFunctions();
    void listFunctions();
private:
    std::string filePath;
};

#endif // DATAFILEMANAGER_H
