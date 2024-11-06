#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string>
#include <vector>

class CommandProcessor {
public:
    CommandProcessor(const std::string& inputPath, const std::string& outputPath);
    void process();

private:
    std::string inputPath;
    std::string outputPath;

    void parseCommands(const std::vector<std::string>& lines);
    void processFunction(const std::string& functionName, const std::vector<std::string>& commands, std::vector<std::string>& processedCommands);
    void replacePlaceholders(std::vector<std::string>& commands, const std::string& placeholder, const std::vector<std::string>& replacements);
    void saveToOutputFile(const std::vector<std::string>& processedCommands);
    std::string promptUserInput(const std::string& prompt);
};

#endif
