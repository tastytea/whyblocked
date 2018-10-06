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
#include "version.hpp"
#include "whyblocked.hpp"
#include "interface_qt.hpp"

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent)
{
    setupUi(this);
    statusBar()->showMessage(tr("Ready"));

    QStandardItemModel *model = new QStandardItemModel;
    tableview->setModel(model);
    populate_tableview(*model);
    model->setHeaderData(0, Qt::Horizontal, tr("User/Instance"));
    model->setHeaderData(1, Qt::Horizontal, tr("Blocked/Silenced?"));
    model->setHeaderData(2, Qt::Horizontal, tr("Reason"));
    tableview->horizontalHeader()->resizeSection(0, 300);
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(action_add, &QAction::triggered, this, &MainWindow::add);
    connect(action_remove, &QAction::triggered, this, &MainWindow::remove);
    connect(action_about, &QAction::triggered, this, &MainWindow::about);

    connect(tableview, &QTableView::clicked, this, &MainWindow::show_details);
    connect(tableview, &QTableView::activated, this, &MainWindow::show_details);
}

void MainWindow::populate_tableview(QStandardItemModel &model)
{
    model.clear();
    result_view result;
    if (view(result))
    {
        for (const std::tuple<string, int, string> &line : result)
        {
            QList<QStandardItem*> items;
            items.append(new QStandardItem(QString::fromStdString((std::get<0>(line)))));
            if (std::get<1>(line) == 1)
            {
                items.append(new QStandardItem(QString(tr("blocked"))));
            }
            else
            {
                items.append(new QStandardItem(QString(tr("silenced"))));
            }
            items.append(new QStandardItem(QString::fromStdString((std::get<2>(line)))));
            model.appendRow(items);
        }
    }
}

void MainWindow::add()
{
    QDialog *dialog_add = new QDialog;
    Ui::DialogAdd ui;
    ui.setupUi(dialog_add);

    if (dialog_add->exec() == 1)
    {
        statusBar()->showMessage(tr("Added nothing. :-P"));
    }
}

void MainWindow::remove()
{
    statusBar()->showMessage(tr("Removed nothing. :-P"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About whyblocked"),
        tr("<p><b>whyblocked</b> %1</p>"
           "<p>Reminds you why you blocked someone.</p>"
           "<p>Sourcecode: <a href=\"https://schlomp.space/tastytea/whyblocked\">"
           "https://schlomp.space/tastytea/whyblocked</a></p>"
           "<p><small>Copyright © 2018 <a href=\"mailto:tastytea@tastytea.de\">tastytea</a>.<br>"
           "Licence GPLv3: <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">"
           "GNU GPL version 3</a>.<br>"
           "This program comes with ABSOLUTELY NO WARRANTY. This is free software,"
           "and you are welcome to redistribute it under certain conditions.</small></p>")
        .arg(global::version));
}

void MainWindow::show_details(QModelIndex index)
{
    const string user = index.sibling(index.row(), 0).data().toString().toStdString();
    result_details result;
    string text = "";

    if (details(user, result))
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("whyblocked");

    MainWindow win;
    win.show();

    return app.exec();
}
