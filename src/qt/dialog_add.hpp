/*  This file is part of whyblocked.
 *  Copyright © 2019 tastytea <tastytea@tastytea.de>
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

#ifndef DIALOG_ADD_HPP
#define DIALOG_ADD_HPP

#include "mainwindow.hpp"
#include "ui_whyblocked_add.h"

class DialogAdd : public QDialog, private Ui::DialogAdd
{
    Q_OBJECT

public:
    explicit DialogAdd(Database &database, QMainWindow *parent = nullptr);
    void set_data(const Database::data &data);

private:
    const Database::data get_data() const;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    MainWindow *_parent;
    Database &_database;

private slots:
    void add_receipt();
    void remove_receipt();
    void accept();

};

#endif  // DIALOG_ADD_HPP
