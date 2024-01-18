#include <string>
#include <iostream>
#include <map>
#include "map.hpp"
#include "game.hpp"

using namespace std;

extern void build_map2box_name();
extern Game *initial_game();
extern vector<string> get_archive();
extern void archiving(const Game *game);
extern Game *read_archive(vector<string> archives, int num);
int lev;

int main()
{
    build_map2box_name();
    while (true)
    {
        cout << "Please choose 'New game' or 'Read archive'(n/r): ";
        string s;
        cin >> s;
        while (s != "n" && s != "r")
        {
            cout << "Please input valid operate(n/r):";
            cin >> s;
        }
        system("cls");
        Game *game;
        if (s == "n")
        {
            cout << "Please choose the level(1~9):";
            cin >> lev;
            while (lev < 1 || lev > 9)
            {
                cout << "Level is invalid, please input a correct level:";
                cin >> lev;
            }
            game = initial_game();
        }
        else
        {

            vector<string> archives = get_archive();
            for (int i = 0; i < archives.size(); i++)
                cout << i + 1 << ": " << archives[i] << endl;
            cout << "Please choose one archive(input the numer of archive):";
            do
            {
                int num;
                cin >> num;
                while (num > archives.size() || num <= 0)
                {
                    cout << endl;
                    cout << "Please input a correct number:";
                    cin >> num;
                }
                cout << endl;
                game = read_archive(archives, num);
                if (game == nullptr)
                    cout << "Please choose another archive(input the numer of archive):";
            } while (game == nullptr);
        }
        system("cls");
        char userInput;
        cin.ignore();
        while (true)
        {
            game->printState();
            cout << "Enter your move (w/a/s/d/z/r/f) or quit (q):";
            userInput = cin.get();
            cin.ignore(); // 忽略换行符
            userInput = tolower(userInput);

            if (userInput == 'f')
            {
                archiving(game);
                system("cls");
                continue;
            }
            // 处理无效输入
            if (!(userInput == 'w' || userInput == 'a' || userInput == 's' || userInput == 'd' || userInput == 'q' || userInput == 'z' || userInput == 'r'))
            {
                std::cout << "Invalid input. Please enter 'w', 'a', 's', 'd', 'r', 'z', or 'q'." << std::endl;
                continue; // 继续循环，等待有效输入
            }
            if (userInput == 'q')
            { // 使用 q 退出
                cout << "\nQuit.\n";
                break;
            }
            else
            {
                system("cls");
                game->handlePlayerMove(userInput);
                if (game->checkWinCondition())
                {
                    system("cls");
                    game->printState();
                    cout << "\nYou won!\n";
                    system("pause");
                    break;
                }
            }
        }
        system("cls");
        cout << "Do you want to continue playing? [Y/n] ";
        char check;
        cin >> check;
        while (check != 'Y' && check != 'n')
        {
            cout << "Please input a valid operation [Y/n]: ";
            cin >> check;
        }
        if (check == 'n')
            break;
        system("cls");
    }

    return 0;
}
