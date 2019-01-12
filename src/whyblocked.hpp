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

    bool add_user(const data &userdata);
    bool remove(const string &user);
    const vector<data> query(const string &sql_query =
                                    "SELECT * FROM blocks;") const;
    bool reload();
    std::vector<data> &get_data();
    const data get_user(const string &user) const;

private:
    std::vector<data> _data;

private:
    const string get_filepath() const;
};

#endif  // WHYBLOCKED_HPP
