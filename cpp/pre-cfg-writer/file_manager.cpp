#include "file_manager.h"
#include <fstream>

bool FileManager::fileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    return file.good();
}

std::vector<std::string> FileManager::readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

void FileManager::writeFile(const std::string& filepath, const std::vector<std::string>& lines) {
    std::ofstream file(filepath);
    for (const auto& line : lines) {
        file << line << "\n";
    }
}
