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
        string answer;
        
        cout << "Would you like to add, remove or view records?\n";
        cout << "Or do you want to get the details of a record?\n";
        cout << "Type add, remove, view or details. Or just the first letter\n";
        cout << ": ";
        cin >> answer;
        switch (answer[0])
        {
            case 'a':
            case 'A':
            {
                cout << "ADD\n";
                break;
            }
            case 'r':
            case 'R':
            {
                cout << "REMOVE\n";
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
                cout << "Which user?\n";
                cin >> answer;
                {
                    sqlite::query q(con, "SELECT * FROM blocks WHERE user = \'" + answer + "\';");
                    boost::shared_ptr<sqlite::result> result = q.get_result();
                    result->next_row();
                    cout << answer << " is ";
                    if (result->get_row_count() == 0)
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
                    sqlite::query q(con, "SELECT * FROM urls WHERE user = \'" + answer + "\';");
                    boost::shared_ptr<sqlite::result> result = q.get_result();
                    while(result->next_row())
                    {
                        cout << result->get_string(1) << '\n';
                    }
                }
                break;
            }
            default:
                cout << "Response not understood.\n";
        }
        // sqlite::execute ins(con, "INSERT INTO TEST VALUES(?, ?, ?);");
        // ins % sqlite::nil % "Hello";
        // ins();

        // sqlite::query q(con, "SELECT * FROM blocks;");
        // boost::shared_ptr<sqlite::result> result = q.get_result();
        // while(result->next_row())
        // {
        //   std::cout << "ID:   " << result->get_int(0) << "\n"
        //             << "Name: " << result->get_string(1) << std::endl;
        // }
    }
    catch(const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
