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
#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "version.hpp"
#include "whyblocked.hpp"
#include "interface_text.hpp"

using std::cout;
using std::cerr;
using std::cin;

// Allow cout to output QStrings
std::ostream &operator <<(std::ostream &stream, const QString &str)
{
   stream << str.toStdString();
   return stream;
}

const string Text::get_answer(const QString &question)
{
    string answer;

    cout << question << ": ";
    std::getline(cin, answer, '\n');

    return answer;
}

bool Text::askblocked()
{
    while (true)
    {
        const string blocked = get_answer(tr("Blocked or silenced?") +
                                          " [b/s]");
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

void Text::askrecipes(const string &user)
{
    while (true)
    {
        const string receipt_yn = get_answer(tr("Add receipt? [y/n]"));
        if (receipt_yn[0] == tr("y")[0] || receipt_yn[0] == tr("Y")[0])
        {
            const string receipt = get_answer(tr("Receipt"));

            if (database::add_receipt(user, receipt))
            {
                cout << tr("Receipt added.") << '\n';
            }
        }
        else if (receipt_yn[0] == tr("n")[0] || receipt_yn[0] == tr("N")[0])
        {
            break;
        }
        else
        {
            continue;
        }
    }
}

void Text::add()
{
    const string user = get_answer(tr("User or instance"));
    int blocked;

    if (askblocked())
    {
        blocked = 1;
    }
    else
    {
        blocked = 0;
    }
    const string reason = get_answer(tr("Reason"));

    if (database::add_block(user, blocked, reason))
    {
        cout << user << " " << tr("added.") << '\n';
    }

    askrecipes(user);
}

void Text::edit()
{
    result_details olddata;
    const string olduser = get_answer(tr("User or instance"));
    if (database::details(olduser, olddata))
    {
        cout << tr("A blank line keeps the former value.") << '\n';
        string newuser = get_answer(tr("Change user or instance to"));
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

        cout << tr("Old reason was:") << " " << std::get<1>(olddata) << '\n';
        string newreason = get_answer(tr("Change reason to"));
        if (newreason.empty())
        {
            newreason = std::get<1>(olddata);
        }

        std::vector<string> newreceipts;
        for (const string &oldreceipt : std::get<2>(olddata))
        {
            cout << tr("Old receipt was:") << " " << oldreceipt << '\n';
            string newreceipt = get_answer(tr("Change receipt to"));
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
            cerr << tr("Could not remove") << " " << olduser << ".\n";
        }
    }
}

void Text::remove()
{
    const string user = get_answer(tr("User or instance"));

    if (database::remove(user))
    {
        cout << user << " " << tr("removed.") << '\n';
    }
}

void Text::view()
{
    result_view result;
    if (database::view(result))
    {
        for (const std::tuple<string, int, string> &line : result)
        {
            if (std::get<1>(line) == 1)
            {
                cout << tr(" Blocked:") << " ";
            }
            else
            {
                cout << tr("Silenced:") << " ";
            }
            cout << std::get<0>(line) << " " << tr("because:") << " ";
            cout << std::get<2>(line) << '\n';
        }
    }
}

void Text::details()
{
    const string user = get_answer(tr("User or instance"));
    {
        result_details result;
        if (database::details(user, result))
        {
            cout << user << " " << tr("is") << " ";
            if (std::get<0>(result) == 1)
            {
                cout << tr("blocked, because:") << " ";
            }
            else if (std::get<0>(result) == 0)
            {
                cout << tr("silenced, because:") << " ";
            }
            cout << std::get<1>(result) << '\n';

            if (!std::get<2>(result).empty())
            {
                cout << tr("Receipts:") << '\n';
                for (const string &url : std::get<2>(result))
                {
                    cout << "  " << url << '\n';
                }
            }
        }
    }
}

void Text::help()
{
    cout << tr(
        "Type add, edit, remove, view or details. Or just the first letter.")
         << '\n';
    cout << tr(
        "Type help or h to show this help. Type quit or q to quit the program.")
         << '\n';
}

Text::Text(QObject *parent) : QObject(parent)
{
    bool keeprunning = true;

    cout << tr("This is Whyblocked") << " " << global::version << ".\n";
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
            {
                cout << tr("Response not understood.") << '\n';
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Whyblocked");
    
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    QTranslator appTranslator;
    appTranslator.load("whyblocked_" + QLocale::system().name(),
                       QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&appTranslator);

    Text t(&app);

    return 0;
}
