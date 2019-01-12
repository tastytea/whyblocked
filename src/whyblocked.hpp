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
        const string user;
        const int blocked;
        const string reason;
        const vector<string> receipts;

        explicit operator bool() const;
    };

    Database();
    bool add_user(const string &user, const int blocked, const string &reason);
    bool add_receipt(const string &user, const string &receipt);
    bool remove(const string &user);
    const vector<data> query(const string &sql_query = "SELECT * FROM blocks;")
        const;

private:
    const string get_filepath() const;
};

#endif  // WHYBLOCKED_HPP
