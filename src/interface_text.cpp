/*  This file is part of whyblocked.
 *  Copyright © 2018 tastytea <tastytea@tastytea.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "version.hpp"
#include "whyblocked.hpp"

using std::cout;
using std::cerr;
using std::cin;

const void print_help()
{
    cout << "Type add, remove, view or details. Or just the first letter.\n";
    cout << "Type help or h to show this help. Type quit or q to quit the program.\n";
}

int main(int argc, char *argv[])
{
    bool keeprunning = true;
    
    cout << "This is whyblocked " << global::version << ".\n";
    print_help();
    while (keeprunning)
    {
        string answer = "";
        cout << ": ";
        cin >> answer;
        switch (answer[0])
        {
            case 'a':
            case 'A':
            {
                string user, reason;
                int blocked = -1;
                cout << "User or instance: ";
                cin >> user;
                while (blocked == -1)
                {
                    cout << "Blocked(b) or silenced(s): ";
                    cin >> answer;
                    if (answer[0] == 'b' || answer[0] == 'B')
                    {
                        blocked = 1;
                    }
                    else if (answer[0] == 's' || answer[0] == 'S')
                    {
                        blocked = 0;
                    }
                }
                cout << "Reason: ";
                cin.ignore();
                std::getline(cin, reason, '\n');

                if (add_block(user, blocked, reason))
                {
                    cout << user << " added.\n";
                }

                while (true)
                {
                    cout << "Add receipt? [y/n] ";
                    cin >> answer;
                    if (answer[0] == 'y' || answer[0] == 'Y')
                    {
                        string url;
                        cout << "URL: ";
                        cin >> url;

                        if (add_url(user, url))
                        {
                            cout << "Receipt added.\n";
                        }
                    }
                    else if (answer[0] == 'n' || answer[0] == 'N')
                    {
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                break;
            }
            case 'r':
            case 'R':
            {
                string user;
                cout << "User or instance: ";
                cin >> user;

                if (remove(user))
                {
                    cout << user << " removed.\n";
                }
                break;
            }
            case 'v':
            case 'V':
            {
                result_view result;
                if (view(result))
                {
                    for (const std::tuple<string, int, string> &line : result)
                    {
                        if (std::get<1>(line) == 1)
                        {
                            cout << " Blocked: ";
                        }
                        else
                        {
                            cout << "Silenced: ";
                        }
                        cout << std::get<0>(line) << " because: ";
                        cout << std::get<2>(line) << '\n';
                    }
                }
                break;
            }
            case 'd':
            case 'D':
            {
                cout << "User or instance: ";
                cin >> answer;
                {
                    result_details result;
                    if (details(answer, result))
                    {
                        cout << answer << " is ";
                        if (std::get<0>(result) == 1)
                        {
                            cout << "blocked, because: ";
                        }
                        else if (std::get<0>(result) == 0)
                        {
                            cout << "silenced, because: ";
                        }
                        cout << std::get<1>(result) << '\n';

                        if (!std::get<2>(result).empty())
                        {
                            cout << "Receipts:\n";
                            for (const string &url : std::get<2>(result))
                            {
                                cout << "  " << url << '\n';
                            }
                        }
                    }
                }
                break;
            }
            case 'h':
            case 'H':
            {
                print_help();
                break;
            }
            case 'q':
            case 'Q':
            {
                keeprunning = false;
                break;
            }
            default:
                cout << "Response not understood.\n";
        }
    }

    return 0;
}
