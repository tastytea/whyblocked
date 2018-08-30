// CC-0, tastytea

#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <basedir.h>
#include <sqlite/connection.hpp>
#include <sqlite/execute.hpp>
#include <sqlite/query.hpp>
#include <boost/shared_ptr.hpp>
#include <version.hpp>

using std::string;
using std::cout;
using std::cerr;
using std::cin;
namespace fs = std::experimental::filesystem;

string get_filepath()
{
    string filepath;
    xdgHandle xdg;
    xdgInitHandle(&xdg);
    filepath = xdgDataHome(&xdg);
    xdgWipeHandle(&xdg);

    filepath += "/whyblock";
    if (!fs::exists(filepath))
    {
        fs::create_directory(filepath);
    }
    filepath += "/database.sqlite";
    if (!fs::exists(filepath))
    {
        sqlite::connection con(filepath);
        sqlite::execute(con, "CREATE TABLE blocks(user TEXT PRIMARY KEY, blocked INTEGER, reason TEXT);", true);
        sqlite::execute(con, "CREATE TABLE urls(user TEXT, url TEXT);", true);
    }

    return filepath;
}

int main(int argc, char *argv[])
{
    try
    {
        sqlite::connection con(get_filepath());
        bool keeprunning = true;
        
        cout << "This is whyblocked " << global::version << ".\n";
        cout << "Type add, remove, view or details. Or just the first letter.\n";
        cout << "Type quit or q to quit the program.\n";
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

                    sqlite::execute ins(con, "INSERT INTO blocks VALUES(?, ?, ?);");
                    ins % user % blocked % reason;
                    ins();

                    while (true)
                    {
                        cout << "Add receipt? [y/n] ";
                        cin >> answer;
                        if (answer[0] == 'y' || answer[0] == 'Y')
                        {
                            string url;
                            cout << "URL: ";
                            cin >> url;

                            sqlite::execute ins(con, "INSERT INTO urls VALUES(?, ?);");
                            ins % user % url;
                            ins();
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

                    sqlite::execute rm_blocks(con, "DELETE FROM blocks WHERE user = ?;");
                    sqlite::execute rm_urls(con, "DELETE FROM urls WHERE user = ?;");
                    rm_blocks % user;
                    rm_urls % user;
                    rm_blocks();
                    rm_urls();

                    break;
                }
                case 'v':
                case 'V':
                {
                    sqlite::query q(con, "SELECT * FROM blocks;");
                    boost::shared_ptr<sqlite::result> result = q.get_result();
                    while(result->next_row())
                    {
                        if (result->get_int(1) == 1)
                        {
                            cout << " Blocked: ";
                        }
                        else
                        {
                            cout << "Silenced: ";
                        }
                        cout << result->get_string(0) << " because: ";
                        cout << result->get_string(2) << '\n';
                    }
                    break;
                }
                case 'd':
                case 'D':
                {
                    cout << "User or instance: ";
                    cin >> answer;
                    {
                        sqlite::query q(con, "SELECT * FROM blocks WHERE user = \'" + answer + "\';");
                        boost::shared_ptr<sqlite::result> result = q.get_result();
                        cout << answer << " is ";
                        if (!result->next_row())
                        {
                            cout << "not in the database.\n";
                            break;
                        }
                        if (result->get_int(1) == 1)
                        {
                            cout << "blocked, because: ";
                        }
                        else if (result->get_int(1) == 0)
                        {
                            cout << "silenced, because: ";
                        }
                        cout << result->get_string(2) << '\n';
                    }
                    {
                        cout << "Receipts:\n";
                        sqlite::query q(con, "SELECT * FROM urls WHERE user = \'" + answer + "\';");
                        boost::shared_ptr<sqlite::result> result = q.get_result();
                        while(result->next_row())
                        {
                            cout << "  " << result->get_string(1) << '\n';
                        }
                    }
                    break;
                case 'q':
                case 'Q':
                    keeprunning = false;
                    break;
                }
                default:
                    cout << "Response not understood.\n";
            }
        }
    }
    catch(const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
