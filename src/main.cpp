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
    cout << "  _____    ______    ____   _    _   _____     _____   ______   ______     ______   __    __           \n"
            " |  __ \\  |  ____|  / ___| | |  | | |  __ \\   / ____| |  ____| |  __  \\   /  __  \\  \\ \\  / /     \n"
            " | |__) | | |__    | |     | |  | | | |__) | | (___   | |__    | |__) /  |  |  |  |  \\ \\/ /          \n"
            " |  _  /  |  __|   | |     | |  | | |  _  /   \\___ \\  |  __|   |  ___ \\  |  |  |  |  / /\\ \\       \n"
            " | | \\ \\  | |____  | |___  | |__| | | | \\ \\   ____) | | |____  | |___) | |  |__|  | / /  \\ \\     \n"
            " |_|  \\_\\ |______|  \\____|  \\____/  |_|  \\_\\ |_____/  |______| |______/   \\______/ /_/    \\_\\  \n";
    cout << endl;
    cout << "Some information:" << endl;
    cout << endl;
    cout << "1.'w' means up" << endl;
    cout << "2.'a' means left" << endl;
    cout << "3.'s' means down" << endl;
    cout << "4.'d' means right" << endl;
    cout << "5.'z' means undo" << endl;
    cout << "6.'r' means reset" << endl;
    cout << "7.'f' means archive" << endl;
    cout << "8. In the map you see, 'BX' or 'bX' (X can be any number) means B box that you can enter it." << endl;
    cout << "9. In the map you see, 'BCX' or 'bCX' (X can be any number) means it is a copy of 'BX' (multiple reference)." << endl;
    cout << "10. In the map you see, 'XIY'(X and Y can be any number) means X-infinite of 'BY'(or 'bY') that you can enter it." << endl;
    cout << "    For example, '1I1' means 1-infinite of B1, '2I3' means 2-infinite of B3,etc." << endl;
    cout << "11. In the map you see, 'E' means epsilon box(When you go out of epsilon box, you will see it)." << endl;
    cout << endl;
    cout << "Some notes:" << endl;
    cout << endl;
    cout << "1.After typing any operation, please follow an enter to submit the operation so that the program could respond to you." << endl;
    cout << "2.After inputing 'f', the program may seems like nothing to happen. But that is normal, it just saved your current game.And you can find it in archive." << endl;
    cout << "3.This CLI is just for testing all basic function and some bonus function, the GUI is still progressing. We may show it on Jan 22nd." << endl;
    cout << endl;

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
            cout << "Some maps information:" << endl;
            cout << "1.A classical recurse box game." << endl;
            cout << "2.Having one B box. Can test enter B box." << endl;
            cout << "3.Having two B box. Can test enter and go out of B box." << endl;
            cout << "4.Having a multiple reference(clone box) example." << endl;
            cout << "5.Another multiple reference(clone box) example." << endl;
            cout << "6.Greedy snake." << endl;
            cout << "7.Have a 1-infinite box." << endl;
            cout << "8.Have a 1-infinite box and a 2-infinite box." << endl;
            cout << "9.An example to test slide into." << endl;
            cout << "10.Another example to test slide into." << endl;
            cout << "11.Multiple epsilon." << endl;
            cout << "12.Mutiple infinite. Feel free to play with infinites!" << endl;
            cout << endl;
            cout << "Please choose the level(1~12):";
            string lev_read;
            cin >> lev_read;
            lev = 0;
            for (char c : lev_read)
                if (!isdigit(c))
                {
                    lev = 2147483647;
                    break;
                }
                else
                    lev = lev * 10 + (c - '0');
            while (lev < 1 || lev > 12)
            {
                cout << "Level is invalid, please input a correct level:";
                cin >> lev_read;
                lev = 0;
                for (char c : lev_read)
                    if (!isdigit(c))
                    {
                        lev = 2147483647;
                        break;
                    }
                    else
                        lev = lev * 10 + (c - '0');
            }
            game = initial_game();
        }
        else
        {
            vector<string> archives = get_archive();
            if (archives.size() == 0)
            {
                cout << "Sorry, there's no archive now." << endl;
                cout << endl;
                cout << "Input any character to go back." << endl;
                system("pause");
                system("cls");
                continue;
            }
            for (int i = 0; i < archives.size(); i++)
            {
                string display = archives[i].substr(0, 4) + "-" + archives[i].substr(4, 2) + "-" + archives[i].substr(6, 2) + " " + archives[i].substr(8, 2) + ":" + archives[i].substr(10, 2) + ":" + archives[i].substr(12);
                cout << i + 1 << ": " << display << endl;
            }
            cout << "Please choose one archive(input the numer of archive):";
            do
            {
                string num_read;
                int num = 0;
                cin >> num_read;
                for (char c : num_read)
                    if (!isdigit(c))
                    {
                        num = 2147483647;
                        break;
                    }
                    else
                        num = num * 10 + (c - '0');
                while (num > archives.size() || num <= 0)
                {
                    cout << endl;
                    cout << "Please input a correct number:";
                    num = 0;
                    cin >> num_read;
                    for (char c : num_read)
                        if (!isdigit(c))
                        {
                            num = 2147483647;
                            break;
                        }
                        else
                            num = num * 10 + (c - '0');
                }
                cout << endl;
                game = read_archive(archives, num);
                if (game == nullptr)
                    cout << "Please choose another archive(input the numer of archive):";
            } while (game == nullptr);
        }
        system("cls");
        string UserInput;
        while (true)
        {
            game->printState();
            cout << "Enter your move (w/a/s/d/z/r/f) or quit (q):";
            cin >> UserInput;
            while ((UserInput.size() != 1) || !(UserInput[0] == 'w' || UserInput[0] == 'a' || UserInput[0] == 's' || UserInput[0] == 'd' || UserInput[0] == 'q' || UserInput[0] == 'z' || UserInput[0] == 'r' || UserInput[0] == 'f'))
            {
                system("cls");
                game->printState();
                cout << "Invalid input. Please enter 'w', 'a', 's', 'd', 'r', 'z', or 'q'." << endl;
                cout << "Enter your move (w/a/s/d/z/r/f) or quit (q):";
                cin >> UserInput;
                continue;
            }
            if (UserInput[0] == 'f')
            {
                archiving(game);
                system("cls");
                continue;
            }
            if (UserInput[0] == 'q')
            { // 使用 q 退出
                cout << "\nQuit.\n";
                break;
            }
            else
            {
                system("cls");
                game->handlePlayerMove(UserInput[0]);
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
        cout << "Do you want to continue playing? [y/n] ";
        string check;
        cin >> check;
        while ((check[0] != 'y' && check[0] != 'n') || (check.size() != 1))
        {
            cout << "Please input a valid operation [y/n]: ";
            cin >> check;
        }
        if (check[0] == 'n')
            break;
        system("cls");
    }

    return 0;
}
