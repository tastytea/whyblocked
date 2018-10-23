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

#include <regex>
#include <QMessageBox>
#include <QDebug>
#include <QTranslator>
#include <QLibraryInfo>
#include "version.hpp"
#include "whyblocked.hpp"
#include "interface_qt.hpp"

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent)
{
    setupUi(this);

    _model = new QStandardItemModel;
    tableview->setModel(_model);
    populate_tableview();
}

void MainWindow::populate_tableview()
{
    _model->clear();
    _model->setHorizontalHeaderLabels(
    {
        tr("User/Instance"),
        tr("Blocked/Silenced"),
        tr("Reason")
    });
    tableview->horizontalHeader()->resizeSection(0, 250);

    result_view result;
    if (database::view(result))
    {
        for (const std::tuple<string, int, string> &line : result)
        {
            add_row(QString::fromStdString(std::get<0>(line)),
                    std::get<1>(line),
                    QString::fromStdString(std::get<2>(line)));
        }
    }

    statusBar()->showMessage(tr("Database loaded."));
}

void MainWindow::add_row(const QString &user, const int &blocked,
                         const QString &reason)
{
    QList<QStandardItem*> items;
    items.append(new QStandardItem(user));
    if (blocked == 1)
    {
        items.append(new QStandardItem(QString(tr("blocked"))));
    }
    else
    {
        items.append(new QStandardItem(QString(tr("silenced"))));
    }
    items.append(new QStandardItem(reason));
    _model->appendRow(items);
}

void MainWindow::add()
{
    DialogAdd *dialog = new DialogAdd(this);
    dialog->show();
}

void MainWindow::edit()
{
    if (tableview->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::warning(this, tr("Invalid selection"),
                             tr("Please select only 1 entry to edit."));
        return;
    }

    DialogAdd *dialog = new DialogAdd(this);
    dialog->setWindowTitle(tr("Edit entry"));

    Dialogdata data;
    QModelIndex index = tableview->selectionModel()->selectedRows().first();
    data.user = index.sibling(index.row(), 0).data().toString().toStdString();
    result_details details;
    database::details(data.user, details);
    if (std::get<0>(details) == true)
    {
        data.blocked = 1;
    }
    else
    {
        data.blocked = 0;
    }
    data.reason = std::get<1>(details);
    data.receipts = std::get<2>(details);

    dialog->set_data(data);
    dialog->setProperty("edit", true);
    dialog->show();
}

void MainWindow::remove()
{
    QItemSelectionModel *selection = tableview->selectionModel();
    if (selection->hasSelection())
    {
        for (auto &row : selection->selectedRows())
        {
            const string user = row.data().toString().toStdString();
            database::remove(user);
            statusBar()->showMessage(tr("Removed %1 from database.")
                                     .arg(QString::fromStdString(user)));
            _model->removeRow(row.row());
        }
        label_receipts->clear();
    }
    else
    {
        statusBar()->showMessage(tr("Select data to remove."));
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Whyblocked"),
        tr("<p><b>Whyblocked</b> %1</p>"
           "<p>Reminds you why you blocked someone.</p>"
           "<p>Sourcecode: <a href=\"https://schlomp.space/tastytea/whyblocked\">"
           "https://schlomp.space/tastytea/whyblocked</a></p>"
           "<p><small>Copyright © 2018 <a href=\"mailto:tastytea@tastytea.de\">tastytea</a>.<br>"
           "Licence GPLv3: <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">"
           "GNU GPL version 3</a>.<br>"
           "This program comes with ABSOLUTELY NO WARRANTY. This is free software, "
           "and you are welcome to redistribute it under certain conditions.</small></p>")
        .arg(global::version));
}

void MainWindow::show_details(QModelIndex index)
{
    const string user = index.sibling(index.row(), 0).data()
                                                     .toString().toStdString();
    result_details result;
    string text = "";

    if (database::details(user, result))
    {
        if (!std::get<2>(result).empty())
        {
            text += "<b>Receipts:</b>";
            for (const string &url : std::get<2>(result))
            {
                text += "<br>" + url;
            }
            text = urls_to_hyperlinks(text);
        }
        label_receipts->setText(QString::fromStdString((text)));
    }
}

const string MainWindow::urls_to_hyperlinks(const string &text)
{
    std::regex re_url("((https?|gopher|ftps?)\\://[^ <]*)");
    return std::regex_replace(text, re_url, "<a href=\"$1\">$1</a>");
}

DialogAdd::DialogAdd(QMainWindow *parent)
: QDialog(parent)
, _parent(static_cast<MainWindow*>(parent))
{
    setupUi(this);
}

const Dialogdata DialogAdd::get_data() const
{
    std::vector<string> receipts;
    for (int row = 0; row <= list_receipts->count() - 1; ++row)
    {
        receipts.push_back(list_receipts->item(row)->text().toStdString());
    }

    Dialogdata data;
    data.user = text_user->text().toStdString();
    data.blocked = radio_blocked->isChecked();
    data.reason = text_reason->text().toStdString();
    data.receipts = receipts;

    return data;
}

const void DialogAdd::set_data(const Dialogdata &data)
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
    Dialogdata data = get_data();

    if (data.user.empty())
    {
        return;
    }
    database::add_block(data.user, data.blocked, data.reason);
    _parent->add_row(QString::fromStdString(data.user),
                     data.blocked,
                     QString::fromStdString(data.reason));
    for (const string &receipt : data.receipts)
    {
        database::add_receipt(data.user, receipt);
    }

    _parent->statusBar()->showMessage(tr("Added %1 to database.")
                                      .arg(QString::fromStdString(data.user)));

    delete this;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Whyblocked");
    
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    QTranslator appTranslator;
    appTranslator.load("whyblocked_" + QLocale::system().name(),
                       QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&appTranslator);

    MainWindow win;
    win.show();

    return app.exec();
}
