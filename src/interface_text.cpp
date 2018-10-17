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
#include <QTranslator>
#include <QLibraryInfo>
#include "version.hpp"
#include "whyblocked.hpp"

using std::cout;
using std::cerr;
using std::cin;

const string get_answer(const string &question)
{
    string answer;

    cout << question << ": ";
    std::getline(cin, answer, '\n');

    return answer;
}

const bool askblocked()
{
    while (true)
    {
        const string blocked = get_answer("Blocked(b) or silenced(s)");
        if (blocked[0] == 'b' || blocked[0] == 'B')
        {
            return true;
        }
        else if (blocked[0] == 's' || blocked[0] == 'S')
        {
            return false;
        }
    }
}

const void askrecipes(const string &user)
{
    while (true)
    {
        const string receipt_yn = get_answer("Add receipt? [y/n]");
        if (receipt_yn[0] == 'y' || receipt_yn[0] == 'Y')
        {
            const string receipt = get_answer("Receipt");

            if (database::add_receipt(user, receipt))
            {
                cout << "Receipt added.\n";
            }
        }
        else if (receipt_yn[0] == 'n' || receipt_yn[0] == 'N')
        {
            break;
        }
        else
        {
            continue;
        }
    }
}

const void add()
{
    const string user = get_answer("User or instance");
    int blocked;

    if (askblocked())
    {
        blocked = 1;
    }
    else
    {
        blocked = 0;
    }
    const string reason = get_answer("Reason");

    if (database::add_block(user, blocked, reason))
    {
        cout << user << " added.\n";
    }

    askrecipes(user);
}

const void edit()
{
    result_details olddata;
    const string olduser = get_answer("User or instance");
    if (database::details(olduser, olddata))
    {
        cout << "A blank line keeps the former value.\n";
        string newuser = get_answer("Change user or instance to");
        if (newuser.empty())
        {
            newuser = olduser;
        }

        int blocked;
        if (askblocked())
        {
            blocked = 1;
        }
        else
        {
            blocked = 0;
        }

        cout << "Old reason was: " << std::get<1>(olddata) << '\n';
        string newreason = get_answer("Change reason to");
        if (newreason.empty())
        {
            newreason = std::get<1>(olddata);
        }

        std::vector<string> newreceipts;
        for (const string &oldreceipt : std::get<2>(olddata))
        {
            cout << "Old receipt was: " << oldreceipt << '\n';
            string newreceipt = get_answer("Change receipt to");
            if (newreceipt.empty())
            {
                newreceipt = oldreceipt;
            }
            newreceipts.push_back(newreceipt);
        }

        if (database::remove(olduser))
        {
            database::add_block(newuser, blocked, newreason);
            if (!newreceipts.empty())
            {
                for (const string &newreceipt : newreceipts)
                {
                    database::add_receipt(newuser, newreceipt);
                }
            }
            askrecipes(newuser);
        }
        else
        {
            cerr << "Could not remove " << olduser << ".\n";
        }
    }
}

const void remove()
{
    const string user = get_answer("User or instance");

    if (database::remove(user))
    {
        cout << user << " removed.\n";
    }
}

const void view()
{
    result_view result;
    if (database::view(result))
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
}

const void details()
{
    const string user = get_answer("User or instance");
    {
        result_details result;
        if (database::details(user, result))
        {
            cout << user << " is ";
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
}

const void help()
{
    cout << "Type add, edit, remove, view or details. Or just the first letter.\n";
    cout << "Type help or h to show this help. Type quit or q to quit the program.\n";
}

int main(int argc, char *argv[])
{
    bool keeprunning = true;
    
    cout << "This is whyblocked " << global::version << ".\n";
    help();
    while (keeprunning)
    {
        string answer = get_answer("");
        switch (answer[0])
        {
            case 'a':
            case 'A':
            {
                add();
                break;
            }
            case 'e':
            case 'E':
            {
                edit();
                break;
            }
            case 'r':
            case 'R':
            {
                remove();
                break;
            }
            case 'v':
            case 'V':
            {
                view();
                break;
            }
            case 'd':
            case 'D':
            {
                details();
                break;
            }
            case 'h':
            case 'H':
            {
                help();
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
