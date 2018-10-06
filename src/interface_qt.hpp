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

#ifndef INTERFACE_QT_HPP
#define INTERFACE_QT_HPP

#include <string>
#include <QMainWindow>
#include <QStandardItemModel>
#include "ui_whyblocked.h"
#include "ui_whyblocked_add.h"

using std::string;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);

private slots:
    void add();
    void remove();
    void about();
    void show_details(QModelIndex index);

private:
    void populate_tableview(QStandardItemModel &model);
    const string urls_to_hyperlinks(const string &text);
};

#endif  // INTERFACE_QT_HPP
