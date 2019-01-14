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

#include <QtCore/qmimedata.h>
#include "dialog_add.hpp"

DialogAdd::DialogAdd(Database &database, QMainWindow *parent)
: QDialog(parent)
, _parent(static_cast<MainWindow*>(parent))
, _database(database)
{
    setupUi(this);
}


void DialogAdd::set_data(const Database::data &data)
{
    text_user->setText(QString::fromStdString(data.user));
    radio_blocked->setChecked(data.blocked);
    radio_silcenced->setChecked(!data.blocked);
    text_reason->setText(QString::fromStdString(data.reason));
    for (const string &receipt : data.receipts)
    {
        QListWidgetItem *item =
            new QListWidgetItem(QString::fromStdString(receipt));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        list_receipts->insertItem(list_receipts->count(), item);
    }
}

const Database::data DialogAdd::get_data() const
{
    std::vector<string> receipts;
    for (int row = 0; row <= list_receipts->count() - 1; ++row)
    {
        receipts.push_back(list_receipts->item(row)->text().toStdString());
    }

    Database::data data;
    data.user = text_user->text().toStdString();
    data.blocked = radio_blocked->isChecked();
    data.reason = text_reason->text().toStdString();
    data.receipts = receipts;

    return data;
}

void DialogAdd::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void DialogAdd::dropEvent(QDropEvent *event)
{
    const QString text = event->mimeData()->text();
    QListWidgetItem *item = new QListWidgetItem(text);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    list_receipts->insertItem(list_receipts->count(), item);
}

void DialogAdd::add_receipt()
{
    QListWidgetItem *item = new QListWidgetItem(tr("Insert receipt here."));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    list_receipts->insertItem(list_receipts->count(), item);
    list_receipts->editItem(item);
}

void DialogAdd::remove_receipt()
{
    for (auto item :list_receipts->selectedItems())
    {
        delete item;
    }
}

void DialogAdd::accept()
{
    if (property("edit").toBool())
    {
        _parent->remove();
    }
    Database::data data = get_data();

    if (!data)
    {
        return;
    }

    _database.add_user(data);
    _parent->add_row(QString::fromStdString(data.user),
                     data.blocked,
                     QString::fromStdString(data.reason));

    delete this;
}
