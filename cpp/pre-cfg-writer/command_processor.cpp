#include "command_processor.h"
#include "file_manager.h"
#include <iostream>
#include <regex>

CommandProcessor::CommandProcessor(const std::string& inputPath, const std::string& outputPath)
    : inputPath(inputPath), outputPath(outputPath) {}

void CommandProcessor::process() {
    if (!FileManager::fileExists(inputPath)) {
        std::cerr << "Error: Input file " << inputPath << " does not exist." << std::endl;
        return;
    }
    
    auto lines = FileManager::readFile(inputPath);
    parseCommands(lines);
}

void CommandProcessor::parseCommands(const std::vector<std::string>& lines) {
    std::vector<std::string> processedCommands;
    std::string functionName;
    std::vector<std::string> currentCommands;

    for (const auto& line : lines) {
        if (line.rfind("//", 0) == 0) {  // 检测注释行作为功能分割
            if (!functionName.empty()) {
                processFunction(functionName, currentCommands, processedCommands);
            }
            functionName = line.substr(2);
            currentCommands.clear();
        } else {
            currentCommands.push_back(line);
        }
    }
    
    if (!functionName.empty()) {
        processFunction(functionName, currentCommands, processedCommands);
    }

    saveToOutputFile(processedCommands);
}

void CommandProcessor::processFunction(const std::string& functionName, const std::vector<std::string>& commands, std::vector<std::string>& processedCommands) {
    std::cout << "绑定" << functionName << "：" << std::endl;
    
    // 获取需要替换的占位符数量
    std::vector<std::string> placeholders;
    for (const auto& command : commands) {
        std::regex placeholderPattern(R"(\[(command\d*|key\d*)\])");
        std::smatch match;
        std::string tempCommand = command;
        
        while (std::regex_search(tempCommand, match, placeholderPattern)) {
            std::string placeholder = match.str(1);
            if (std::find(placeholders.begin(), placeholders.end(), placeholder) == placeholders.end()) {
                placeholders.push_back(placeholder);
            }
            tempCommand = match.suffix().str();
        }
    }

    std::vector<std::string> replacements;
    for (size_t i = 0; i < placeholders.size(); ++i) {
        std::string input = promptUserInput("绑定" + functionName + std::to_string(i + 1) + "：");
        replacements.push_back(input);
    }

    auto commandCopy = commands;
    for (size_t i = 0; i < placeholders.size(); ++i) {
        replacePlaceholders(commandCopy, placeholders[i], {replacements[i]});
    }

    processedCommands.push_back("//" + functionName);
    processedCommands.insert(processedCommands.end(), commandCopy.begin(), commandCopy.end());
}

void CommandProcessor::replacePlaceholders(std::vector<std::string>& commands, const std::string& placeholder, const std::vector<std::string>& replacements) {
    size_t replacementIndex = 0;
    for (auto& command : commands) {
        std::regex pattern("\\[" + placeholder + "\\]");
        std::smatch match;

        while (std::regex_search(command, match, pattern) && replacementIndex < replacements.size()) {
            command = std::regex_replace(command, pattern, replacements[replacementIndex], std::regex_constants::format_first_only);
            replacementIndex++;
        }

        if (replacementIndex >= replacements.size()) break;
    }
}

void CommandProcessor::saveToOutputFile(const std::vector<std::string>& processedCommands) {
    if (FileManager::fileExists(outputPath)) {
        auto existingCommands = FileManager::readFile(outputPath);

        std::vector<std::string> mergedCommands;
        for (const auto& line : existingCommands) {
            if (line.find("//") == 0 && std::find(processedCommands.begin(), processedCommands.end(), line) != processedCommands.end()) {
                continue;
            }
            mergedCommands.push_back(line);
        }

        mergedCommands.insert(mergedCommands.end(), processedCommands.begin(), processedCommands.end());
        FileManager::writeFile(outputPath, mergedCommands);
    } else {
        FileManager::writeFile(outputPath, processedCommands);
    }
}

std::string CommandProcessor::promptUserInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}
