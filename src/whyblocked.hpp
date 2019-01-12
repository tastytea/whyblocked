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

#ifndef WHYBLOCKED_HPP
#define WHYBLOCKED_HPP

#include <vector>
#include <tuple>
#include <string>

using std::string;
using std::vector;

class Database
{
public:
    struct data
    {
        string user;
        bool blocked;
        string reason;
        vector<string> receipts;

        explicit operator bool() const;
    };

    static bool add_user(const string &user, const bool blocked,
                         const string &reason);
    static bool add_receipt(const string &user, const string &receipt);
    static bool remove(const string &user);
    static const vector<data> query(const string &sql_query =
                                    "SELECT * FROM blocks;");

private:
    static const string get_filepath();
};

#endif  // WHYBLOCKED_HPP
