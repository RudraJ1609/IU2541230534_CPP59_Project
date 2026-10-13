#include "FileManager.h"
#include <fstream>
using namespace std;

bool FileManager::writeLines(const string& fileName, const string lines[], int count) const {
    ofstream out(fileName.c_str());
    if (!out) return false;
    for (int i = 0; i < count; i++) out << lines[i] << "\n";
    return true;
}

int FileManager::readLines(const string& fileName, string*& lines) const {
    lines = nullptr;
    ifstream in(fileName.c_str());
    if (!in) return 0;
    string line;
    int count = 0;
    while (getline(in, line)) if (!line.empty()) count++;   // first pass: count lines
    if (count == 0) return 0;

    lines = new string[count];
    in.clear();
    in.seekg(0);
    int i = 0;
    while (i < count && getline(in, line))                    // second pass: store lines
        if (!line.empty()) lines[i++] = line;
    return count;
}

int FileManager::split(const string& line, string parts[], int maxParts) const {
    int n = 0;
    size_t start = 0;
    while (n < maxParts) {
        size_t pos = line.find('|', start);
        if (pos == string::npos) { parts[n++] = line.substr(start); break; }
        parts[n++] = line.substr(start, pos - start);
        start = pos + 1;
    }
    return n;
}
