#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    static bool fileExists(const std::string& filepath);
    static std::vector<std::string> readFile(const std::string& filepath);
    static void writeFile(const std::string& filepath, const std::vector<std::string>& lines);
};

#endif
