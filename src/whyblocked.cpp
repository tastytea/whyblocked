/*  This file is part of whyblocked.
 *  Copyright © 2018, 2019 tastytea <tastytea@tastytea.de>
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

Database::data::operator bool() const
{
    return !user.empty();
}

const string Database::get_filepath()
{
    fs::path filepath;
    xdgHandle xdg;
    xdgInitHandle(&xdg);
    filepath = xdgDataHome(&xdg);
    xdgWipeHandle(&xdg);

    filepath /= "whyblocked";
    if (!fs::exists(filepath))
    {
        fs::create_directories(filepath);
    }
    filepath /= "database.sqlite";
    if (!fs::exists(filepath))
    {
        sqlite::connection con(filepath);
        sqlite::execute(con, "CREATE TABLE blocks(user TEXT PRIMARY KEY, "
                        "blocked INTEGER, reason TEXT);", true);
        sqlite::execute(con, "CREATE TABLE urls(user TEXT, url TEXT);", true);
    }

    return filepath;
}

bool Database::add_user(const string &user, const bool blocked,
                        const string &reason)
{
    try
    {
        int blocked_int = 0;
        if (blocked)
        {
            blocked_int = 1;
        }
        sqlite::connection con(get_filepath());
        sqlite::execute ins(con, "INSERT INTO blocks VALUES(?, ?, ?);");
        ins % user % blocked_int % reason;
        ins();
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool Database::add_receipt(const string &user, const string &receipt)
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

bool Database::remove(const string &user)
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

const vector<Database::data> Database::query(const string &sql_query)
{
    try
    {
        sqlite::connection con(get_filepath());
        sqlite::query q_blocks(con, sql_query);
        sqlite::result_type res_blocks = q_blocks.get_result();
        std::vector<data> result;

        while(res_blocks->next_row())
        {
            const string user = res_blocks->get_string(0);
            const int blocked = res_blocks->get_int(1);
            const string reason = res_blocks->get_string(2);
            bool blocked_bool = false;
            if (blocked == 1)
            {
                blocked_bool = true;
            }

            sqlite::query q_urls(con,
                "SELECT * FROM urls WHERE user = \'" + user + "\';");
            sqlite::result_type res_urls = q_urls.get_result();
            vector<string> receipts;
            while(res_urls->next_row())
            {
                receipts.push_back(res_urls->get_string(1));
            }

            result.push_back(
            {
                user,
                blocked_bool,
                reason,
                receipts
            });
        }

        return result;
    }
    catch (const std::exception &e)
    {
        cerr << "An error occurred: " << e.what() << std::endl;
        return {};
    }
}
