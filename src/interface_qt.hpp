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
#include <memory>
#include <array>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QDialog>
#include <QtGui/qevent.h>
#include "xdgcfg.hpp"
#include "ui_whyblocked.h"
#include "ui_whyblocked_add.h"

using std::string;

struct Dialogdata
{
    string user = "";
    bool blocked = true;
    string reason = "";
    std::vector<string> receipts = {};
};

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

private slots:
    void add();
    void edit();
    void about();
    void show_details(QModelIndex index);
    void populate_tableview();

};

class DialogAdd : public QDialog, private Ui::DialogAdd
{
    Q_OBJECT

public:
    explicit DialogAdd(QMainWindow *parent = nullptr);
    const void set_data(const Dialogdata &data);

private:
    const Dialogdata get_data() const;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    MainWindow *_parent;

private slots:
    void add_receipt();
    void remove_receipt();
    void accept();

};

#endif  // INTERFACE_QT_HPP
