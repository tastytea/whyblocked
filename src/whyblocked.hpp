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

#ifndef WHYBLOCKED_HPP
#define WHYBLOCKED_HPP

#include <vector>
#include <tuple>
#include <string>

using std::string;
using result_view = std::vector<std::tuple<string, int, string>>;
using result_details = std::tuple<int, string, std::vector<string>>;

const string get_filepath();
namespace database
{
    const bool add_block(const string &user, const int blocked,
                         const string &reason);
    const bool add_receipt(const string &user, const string &receipt);
    const bool remove(const string &user);
    const bool view(result_view &result, const string &sql_query = "");
    const bool details(const string &user, result_details &result);
}

#endif  // WHYBLOCKED_HPP
