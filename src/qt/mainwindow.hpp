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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <string>
#include <memory>
#include <array>
#include <vector>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QDialog>
#include <QtGui/qevent.h>
#include "../xdgcfg.hpp"
#include "../whyblocked.hpp"
#include "ui_whyblocked.h"

using std::string;
using std::vector;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();
    void add_row(const QString &user, const int &blocked,
                 const QString &reason);

public slots:
    void remove();

private:
    const string urls_to_hyperlinks(const string &text);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    QStandardItemModel *_model;
    xdgcfg _config;
    std::array<int, 3> _headersize;
    Database _database;
    std::vector<Database::data> &_dbdata;

private slots:
    void add();
    void edit();
    void about();
    void show_details(QModelIndex index);
    void populate_tableview(const vector<Database::data> &entries);
    void reload();
    void find();
    void update_search(const QString &text);
    void update_search();

};

#endif  // MAINWINDOW_HPP
