#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "game.hpp"
#include "map.hpp"
#include <iomanip>
#include <chrono>
namespace fs = filesystem;
extern int lev;
extern Game *initial_game();

bool check_extension_name(string filename)
{
    if (!filename.compare(filename.length() - 4, 4, ".arc"))
        return true;
    return false;
}

vector<string> get_archive()
{
    string foldername = "archive";
    fs::path folderPath = fs::current_path() / foldername;

    vector<string> archives;

    if (fs::exists(folderPath) && fs::is_directory(folderPath))
        for (const auto &entry : fs::directory_iterator(folderPath))
        {
            if (check_extension_name(entry.path().filename().string()))
            {
                string filename = entry.path().filename().string();
                string name = filename.substr(0, filename.length() - 4);
                archives.push_back(name);
            }
        }
    return archives;
}

string get_Timestamp()
{
    auto now = chrono::system_clock::now();

    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm *timeInfo = localtime(&currentTime);

    ostringstream oss;
    oss << put_time(timeInfo, "%Y%m%d%H%M%S");
    return oss.str();
}

unsigned long long get_Timestamp_check_code(string Timestamp)
{
    unsigned long long sum = 0;
    for (char c : Timestamp)
    {
        unsigned long long num = c - '0';
        num = num * (Timestamp[(num * num + 131) % Timestamp.size()]) + Timestamp[(num * num * num + 13131) % Timestamp.size()];
        sum += num;
    }
    return sum;
}

void archiving(const Game *game)
{
    string Timestamp = get_Timestamp();
    string filename = Timestamp + ".arc";
    string foldername = "archive";
    fs::path folderPath = fs::current_path() / foldername;
    fs::path filePath = folderPath / filename;

    // 如果不存在，就创建文件夹
    if (!fs::exists(folderPath))
    {
        fs::create_directory(folderPath);
    }

    unsigned long long Timestamp_check_code = get_Timestamp_check_code(Timestamp);

    ofstream fileStream(filePath);
    if (fileStream.is_open())
    {
        // 打印时间戳校验码
        fileStream << Timestamp_check_code << endl;
        // 打印等级
        fileStream << lev << endl;
        // 打印地图
        unsigned long long map_check_code = (((lev + 131) * (lev + 131)) ^ 0x55AA);
        for (const auto &print : game->get_map2name())
        {
            Map *now_map = print.first;
            for (int i = 0; i < now_map->mapTable.size(); i++)
            {
                for (int j = 0; j < now_map->mapTable[i].size(); j++)
                {
                    string s = now_map->mapTable[i][j]->get_mark();
                    fileStream << s << " ";
                    for (char c : s)
                        map_check_code += ((c * c) ^ 0x55AA);
                }
                fileStream << endl;
            }
        }
        fileStream << "map_end" << endl;
        // 打印地图检验码
        fileStream << map_check_code << endl;

        unsigned long long step_check_code = 0;
        for (int step : (game->get_step_rec()))
        {
            fileStream << step << endl;
            step_check_code += ((step * (step + 100) * (step + 1000)) ^ 131);
        }
        fileStream << "step_end" << endl;
        fileStream << step_check_code << endl;
        fileStream.close();
        cout << "You have archived successfully!" << endl;
    }
    else
    {
        cerr << "Error: Can not archive!" << endl;
    }
}

Game *read_archive(vector<string> archives, int num)
{
    string foldername = "archive";
    string filename = archives[num - 1];
    unsigned long long Timestamp_check_code = get_Timestamp_check_code(filename);
    filename += ".arc";
    fs::path folderPath = fs::current_path() / foldername;
    fs::path filePath = folderPath / filename;
    ifstream file(filePath);

    if (!file.is_open())
    {
        cout << "Can not open this archive,this archive may be dameged." << endl;
        return nullptr;
    }
    // 读取校验码
    unsigned long long Timestamp_check_code_read;
    file >> Timestamp_check_code_read;
    if (Timestamp_check_code_read != Timestamp_check_code)
    {
        cout << "Can not open this archive,this archive may be dameged." << endl;
        return nullptr;
    }

    file >> lev;
    if (lev <= 0 || lev > 9)
    {
        cout << "Can not open this archive,this archive may be dameged." << endl;
        lev = 0;
        return nullptr;
    }
    unsigned long long map_check_code = (((lev + 131) * (lev + 131)) ^ 0x55AA);

    string info;

    while (file >> info)
    {
        if (info == "map_end")
            break;
        for (char c : info)
            map_check_code += ((c * c) ^ 0x55AA);
    }
    unsigned long long map_check_code_read;
    file >> map_check_code_read;
    if (map_check_code_read != map_check_code)
    {
        cout << "Can not open this archive,this archive may be dameged." << endl;
        lev = 0;
        return nullptr;
    }
    vector<int> step;
    unsigned long long step_check_code = 0;
    while (file >> info)
    {
        if (info == "step_end")
            break;
        int ste = info[0] - '0';
        if (ste < 1 || ste > 4)
        {
            cout << "Can not open this archive,this archive may be dameged." << endl;
            lev = 0;
            return nullptr;
        }
        step.push_back(ste);
        step_check_code += ((ste * (ste + 100) * (ste + 1000)) ^ 131);
    }
    unsigned long long step_check_code_read;
    file >> step_check_code_read;
    if (step_check_code_read != step_check_code)
    {
        cout << "Can not open this archive,this archive may be dameged." << endl;
        lev = 0;
        return nullptr;
    }

    Game *game = initial_game();
    for (int dir : step)
    {
        char op;
        switch (dir)
        {
        case 1:
            op = 'w';
            break;
        case 2:
            op = 'd';
            break;
        case 3:
            op = 's';
            break;
        case 4:
            op = 'a';
            break;
        default:
            break;
        }
        game->handlePlayerMove(op);
    }
    return game;
}