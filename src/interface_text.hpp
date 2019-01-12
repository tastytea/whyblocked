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

#ifndef INTERFACE_TEXT_HPP
#define INTERFACE_TEXT_HPP

#include <string>
#include <QtCore>

using std::string;

class Text : public QObject
{
    Q_OBJECT

public:
    explicit Text(QObject *parent = nullptr);

private:
    const string get_answer(const QString &question);
    bool askblocked();
    void askrecipes(const string &user);
    void add();
    void edit();
    void remove();
    void view();
    void details();
    void help();
};

#endif  // INTERFACE_TEXT_HPP
