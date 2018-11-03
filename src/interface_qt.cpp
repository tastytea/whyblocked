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
#include <array>
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <QTranslator>
#include <QLibraryInfo>
#include <QtCore/qmimedata.h>
#include <libconfig.h++>
#include "version.hpp"
#include "whyblocked.hpp"
#include "interface_qt.hpp"

MainWindow::MainWindow(QMainWindow *parent)
: QMainWindow(parent)
, _config("whyblocked.cfg")
, _headersize({ 250, 125, 125 })
{
    setupUi(this);

    _model = new QStandardItemModel;
    tableview->setModel(_model);

    if (_config.read() == 0)
    {
        libconfig::Setting &root = _config.get_cfg().getRoot();
        string key;

        key = "size";
        if (root.exists(key) && root[key.c_str()].isArray())
        {
            this->resize(root[key.c_str()][0], root[key.c_str()][1]);
        }

        key = "toolbar_position";
        if (root.exists(key))
        {
            const string value = root[key.c_str()].c_str();
            if (value == "top")
            {
                this->removeToolBar(toolbar);
                this->addToolBar(Qt::TopToolBarArea, toolbar);
            }
            else if (value == "right")
            {
                this->removeToolBar(toolbar);
                this->addToolBar(Qt::RightToolBarArea, toolbar);
            }
            else if (value == "bottom")
            {
                this->removeToolBar(toolbar);
                this->addToolBar(Qt::BottomToolBarArea, toolbar);
            }
            else if (value == "left")
            {
                this->removeToolBar(toolbar);
                this->addToolBar(Qt::LeftToolBarArea, toolbar);
            }
        }

        key = "toolbar_visible";
        if (root.exists(key))
        {
            toolbar->setVisible(root[key.c_str()]);
        }

        key = "table_headers";
        if (root.exists(key) && root[key.c_str()].isArray())
        {
            const libconfig::Setting &value = root[key.c_str()];
            _headersize = { value[0], value[1], value[2] };
        }
    }

    populate_tableview();
}

MainWindow::~MainWindow()
{
    libconfig::Setting &root = _config.get_cfg().getRoot();
    // We can't add an element that already exists, so we delete it beforehand.
    for (const string &key :
         { "size", "toolbar_position", "toolbar_visible", "table_headers" })
    {
        if (root.exists(key))
        {
            root.remove(key);
        }
    }
    
    libconfig::Setting &size = root.add("size", libconfig::Setting::TypeArray);
    size.add(libconfig::Setting::TypeInt) = this->width();
    size.add(libconfig::Setting::TypeInt) = this->height();

    libconfig::Setting &pos = root.add("toolbar_position",
                                       libconfig::Setting::TypeString);
    if (toolbar->orientation() == Qt::Orientation::Horizontal)
    {
        if (toolbar-> geometry().top() < 100)
        {
            pos = "top";
        }
        else
        {
            pos = "bottom";
        }
    }
    else
    {
        if (toolbar->geometry().left() == 0)
        {
            pos = "left";
        }
        else
        {
            pos = "right";
        }
    }

    root.add("toolbar_visible", libconfig::Setting::TypeBoolean)
        = !toolbar->isHidden();

    libconfig::Setting &headers = root.add("table_headers",
                                           libconfig::Setting::TypeArray);
    headers.add(libconfig::Setting::TypeInt) =
        tableview->horizontalHeader()->sectionSize(0);
    headers.add(libconfig::Setting::TypeInt) =
        tableview->horizontalHeader()->sectionSize(1);
    headers.add(libconfig::Setting::TypeInt) =
        tableview->horizontalHeader()->sectionSize(2);

    _config.write();
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
    tableview->horizontalHeader()->resizeSection(0, _headersize[0]);
    tableview->horizontalHeader()->resizeSection(1, _headersize[1]);
    tableview->horizontalHeader()->resizeSection(2, _headersize[2]);

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
            text += string("<b>") + tr("Receipts:").toStdString() + "</b>";
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

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    string text = event->mimeData()->text().toStdString();
    const std::array<const std::regex, 4> fediverse =
    {
        std::regex("https://([^/]+)/@([^/]+)"),         // Mastodon
        std::regex("https://([^/]+)/profile/([^/]+)"),  // Friendica
        std::regex("https://([^/]+)/users/([^/]+)"),    // Pleroma
        std::regex("https://([^/]+)/([^/]+)")           // Gnusocial
    };
    std::smatch match;

    for (const std::regex &re : fediverse)
    {
        std::regex_match(text, match, re);
        const string instance = match[1];
        const string user = match[2];
        if (!instance.empty() && !user.empty())
        {
            text = '@' + user + '@' + instance;
            break;
        }
    }

    DialogAdd *dialog = new DialogAdd(this);
    Dialogdata data;
    data.user = text;
    dialog->set_data(data);
    dialog->show();
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
