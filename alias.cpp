#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

// 创建 autoexec.cfg 文件
void createAutoexecFile(const std::string& filePath) {
    std::ofstream file(filePath);
    if (file) {
        file << "// This is autoexec.cfg\n"; // 添加文件头
    }
}

// 检查文件是否存在
bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

// 读取文件内容
std::vector<std::string> readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// 写入文件
void writeFile(const std::string& filePath, const std::vector<std::string>& lines) {
    std::ofstream file(filePath);
    for (const auto& line : lines) {
        file << line << "\n";
    }
}

// 修改绑定
void modifyBindings(std::vector<std::string>& lines, const std::string& key, const std::string& aliasCommand) {
    std::string bindLine = "bind " + key + " " + aliasCommand;

    // 检查是否存在旧的 bind 命令
    auto it = std::remove_if(lines.begin(), lines.end(), [&](const std::string& line) {
        return line.find("bind " + key) == 0; // 检查当前行是否以 "bind " 开头
    });

    // 删除所有匹配的行
    lines.erase(it, lines.end());

    // 添加新的绑定行
    lines.push_back(bindLine);
}

// 删除指定功能的相关内容
void removeOldFunction(std::vector<std::string>& lines, const std::string& funcName) {
    bool removeLines = false;
    auto it = lines.begin();

    while (it != lines.end()) {
        if (it->find(funcName) != std::string::npos) {
            removeLines = true; // 找到开始行，开始删除
        }

        // 如果当前行是另一个功能的开始，则停止删除
        if (removeLines && it->find("//") == 0 && it->find(funcName) == std::string::npos) {
            break; // 找到下一个功能，停止删除
        }

        if (removeLines) {
            it = lines.erase(it); // 删除当前行
        } else {
            ++it; // 移动到下一行
        }
    }
}

// 处理一键跳投功能
void handleJumpThrow(std::vector<std::string>& lines) {
    std::string jumpKey;

    // 获取用户输入
    std::cout << "请设置一键跳投的按键：";
    std::cin >> jumpKey;

    std::string jumpThrowAlias = "alias jt \"echo \"Fusion/jumpthrow\" | exec;\""; // 不再转义双引号

    // 删除旧的跳投功能
    removeOldFunction(lines, "//jumpthrow");

    // 添加新的 alias 和 bind
    lines.push_back("//jumpthrow");
    lines.push_back(jumpThrowAlias);
    modifyBindings(lines, jumpKey, "jt"); // 将按键绑定到 jt
}

// 处理滚轮跳功能
void handleBunnyHopping(std::vector<std::string>& lines) {
    std::string wheelKey;
    std::cout << "请设置滚轮跳的按键：";
    std::cout << "\n选择：\n1. 上滚轮\n2. 下滚轮\n";
    int choice;
    std::cin >> choice;

    // 设置滚轮绑定
    if (choice == 1) {
        wheelKey = "mwheelup";
    } else if (choice == 2) {
        wheelKey = "mwheeldown";
    } else {
        std::cout << "无效选择！\n";
        return;
    }

    std::string bhAliasStart = "alias +bh \"echo \"Fusion/bhop\" | exec;\"";
    std::string bhAliasEnd = "alias -bh \"echo \"Fusion/bhopm\" | exec;\"";

    // 删除旧的滚轮跳功能
    removeOldFunction(lines, "//bunnyhopping");

    // 添加新的 alias 和 bind
    lines.push_back("//bunnyhopping");
    lines.push_back(bhAliasStart);
    lines.push_back(bhAliasEnd);
    modifyBindings(lines, wheelKey, "+bh"); // 将滚轮按键绑定到 +bh
}

// 处理轮盘发刀功能
void handleKnifeWheel(std::vector<std::string>& lines) {
    std::string knifeCommand;

    // 获取用户输入的指令
    std::cout << "请设置轮盘发刀指令（例如knife）：";
    std::cin >> knifeCommand;

    // 更新轮盘发刀部分内容
    std::string knifeAlias = "alias " + knifeCommand + " \"exec knifewheel/mainwheel;echo;echo;echo 按P打开轮盘，O关闭轮盘，将鼠标移动到你需要的刀后按E选择;echo;echo\"";

    // 删除旧的轮盘发刀功能
    removeOldFunction(lines, "//knifewheel");

    // 添加新的轮盘发刀内容
    lines.push_back("//knifewheel");
    lines.push_back("unbind o;");
    lines.push_back("unbind p;");
    lines.push_back(knifeAlias);
}

// 处理练习房功能
void handlePracticeRoom(std::vector<std::string>& lines) {
    std::string practiceKey;

    // 获取用户输入
    std::cout << "请设置练习房的按键：";
    std::cin >> practiceKey;

    // 更新练习房指令
    std::string practiceAlias = "bind " + practiceKey + " \"sv_cheats 1;ammo_grenade_limit_total 6;bot_kick;sv_infinite_ammo 1;mp_startmoney 99999;mp_ignore_round_win_conditions 1;mp_freezetime 0;mp_buy_anywhere 1;mp_buytime 9999;mp_restartgame 1;sv_grenade_trajectory 1;mp_maxmoney 99999;mp_respawn_on_death_t 1;mp_respawnwavetime_t 0;mp_respawn_on_death_ct 1;mp_respawnwavetime_ct 0;mp_autoteambalance 0;mp_limitteams 0;sv_grenade_trajectory_prac_pipreview 1;sv_grenade_trajectory_prac_trailtime 4\"";

    // 删除旧的练习房功能
    removeOldFunction(lines, "//practice");

    // 添加新的练习房内容
    lines.push_back("//practice");
    lines.push_back(practiceAlias);
}

// 处理道具练习功能
void handleItemPractice(std::vector<std::string>& lines) {
    std::string itemCommand;

    // 获取用户输入的指令
    std::cout << "请输入道具练习指令（例如menu）：";
    std::cin >> itemCommand;

    // 更新道具练习部分内容
    std::string itemAlias = "alias " + itemCommand + " \"exec Fusion/menu\"";

    // 删除旧的道具练习功能
    removeOldFunction(lines, "//menu");

    // 添加新的道具练习内容
    lines.push_back("//menu");
    lines.push_back(itemAlias);
}

// 主菜单
void mainMenu(std::vector<std::string>& lines) {
    int choice;
    do {
        std::cout << "选择功能：\n";
        std::cout << "1. 一键跳投\n";
        std::cout << "2. 加强滚轮跳\n";
        std::cout << "3. 轮盘发刀（本地房用）\n";
        std::cout << "4. 设置练习房\n";
        std::cout << "5. 道具练习\n"; // 添加道具练习选项
        std::cout << "0. 保存并退出\n";
        std::cout << "请输入选项：";
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleJumpThrow(lines);
                break;
            case 2:
                handleBunnyHopping(lines);
                break;
            case 3:
                handleKnifeWheel(lines);
                break;
            case 4:
                handlePracticeRoom(lines);
                break;
            case 5:
                handleItemPractice(lines); // 处理道具练习
                break;
            case 0:
                std::cout << "退出程序。\n";
                break;
            default:
                std::cout << "无效选项，请重新选择。\n";
                break;
        }
    } while (choice != 0);
}

// 主函数
int main() {
    // 设置控制台为 UTF-8 编码
    system("chcp 65001 > nul");

    std::string cfgPath = "game/csgo/cfg/autoexec.cfg";

    // 检查文件是否存在
    if (!fileExists(cfgPath)) {
        createAutoexecFile(cfgPath); // 创建文件
    }

    auto lines = readFile(cfgPath); // 读取文件内容
    mainMenu(lines); // 显示主菜单
    writeFile(cfgPath, lines); // 写回文件

    return 0;
}
