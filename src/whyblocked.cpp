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
#include <string>
#include <vector>
#include <tuple>
#include <experimental/filesystem>
#include <basedir.h>
#include <sqlite/connection.hpp>
#include <sqlite/execute.hpp>
#include <sqlite/query.hpp>
#include "whyblocked.hpp"

using std::cerr;
namespace fs = std::experimental::filesystem;

const string get_filepath()
{
    string filepath;
    xdgHandle xdg;
    xdgInitHandle(&xdg);
    filepath = xdgDataHome(&xdg);
    xdgWipeHandle(&xdg);

    filepath += "/whyblocked";
    if (!fs::exists(filepath))
    {
        fs::create_directories(filepath);
    }
    filepath += "/database.sqlite";
    if (!fs::exists(filepath))
    {
        sqlite::connection con(filepath);
        sqlite::execute(con, "CREATE TABLE blocks(user TEXT PRIMARY KEY, "
                        "blocked INTEGER, reason TEXT);", true);
        sqlite::execute(con, "CREATE TABLE urls(user TEXT, url TEXT);", true);
    }

    return filepath;
}

const bool database::add_block(const string &user, const int blocked,
                               const string &reason)
{
    try
    {
        sqlite::connection con(get_filepath());
        sqlite::execute ins(con, "INSERT INTO blocks VALUES(?, ?, ?);");
        ins % user % blocked % reason;
        ins();
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }

    return true;
}

const bool database::add_receipt(const string &user, const string &receipt)
{
    try
    {
        sqlite::connection con(get_filepath());
        sqlite::execute ins(con, "INSERT INTO urls VALUES(?, ?);");
        ins % user % receipt;
        ins();
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }

    return true;
}

const bool database::remove(const string &user)
{
    try
    {
        sqlite::connection con(get_filepath());
        sqlite::execute rm_blocks(con, "DELETE FROM blocks WHERE user = ?;");
        sqlite::execute rm_urls(con, "DELETE FROM urls WHERE user = ?;");
        rm_blocks % user;
        rm_urls % user;
        rm_blocks();
        rm_urls();
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }

    return true;
}

const bool database::view(result_view &result)
{
    try
    {
        sqlite::connection con(get_filepath());
        sqlite::query q(con, "SELECT * FROM blocks;");
        sqlite::result_type res = q.get_result();
        while(res->next_row())
        {
            result.push_back(
            {
                res->get_string(0),
                res->get_int(1),
                res->get_string(2)
            });
        }
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }

    return true;
}

const bool database::details(const string &user, result_details &result)
{
    try
    {
        sqlite::connection con(get_filepath());
        sqlite::query q_blocks(con, "SELECT * FROM blocks WHERE user = \'" + user + "\';");
        sqlite::result_type res_blocks = q_blocks.get_result();

        sqlite::query q_urls(con, "SELECT * FROM urls WHERE user = \'" + user + "\';");
        sqlite::result_type res_urls = q_urls.get_result();

        if (!res_blocks->next_row())
        {
            cerr << user << " is not in the database.\n";
            return false;
        }

        std::vector<string> urls;
        while (res_urls->next_row())
        {
            urls.push_back(res_urls->get_string(1));
        }

        result =
        {
            res_blocks->get_int(1),
            res_blocks->get_string(2),
            urls
        };
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }

    return true;
}
