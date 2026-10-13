#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>

// Simple text-file helper: one record per line, fields separated by '|'
class FileManager {
public:
    bool writeLines(const std::string& fileName, const std::string lines[], int count) const;
    int readLines(const std::string& fileName, std::string*& lines) const;   // caller must delete[] lines
    int split(const std::string& line, std::string parts[], int maxParts) const;
};
#endif
