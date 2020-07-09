/*  This file is part of whyblocked.
 *  Copyright © 2019, 2020 tastytea <tastytea@tastytea.de>
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

#include "mainwindow.hpp"

#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Whyblocked");

    QTranslator qtTranslator;
    qtTranslator.load(QLocale(), "qt", "_",
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    app.installTranslator(&qtTranslator);
    QTranslator appTranslator;
    appTranslator.load(QLocale(), "whyblocked", "_",
                       QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&appTranslator);

    MainWindow win;
    win.show();

    return app.exec();
}
