//------------------------------------------------------------------------------
//
//  Intan Technologies RHX Data Acquisition Software
//  Version 3.5.2
//
//  Copyright (c) 2020-2026 Intan Technologies
//
//  This file is part of the Intan Technologies RHX Data Acquisition Software.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//  This software is provided 'as-is', without any express or implied warranty.
//  In no event will the authors be held liable for any damages arising from
//  the use of this software.
//
//  See <https://www.intantech.com> for documentation and product information.
//
//------------------------------------------------------------------------------

#include <QApplication>
#include <QCommandLineParser>
#include <QStyleFactory>
#include "boardselectdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Intan RHX Software. Run with --settings=startup.ini to skip startup dialog.");
    parser.addHelpOption();
    app.setApplicationVersion(SoftwareVersion);
    parser.addVersionOption();

    QCommandLineOption settingsOption(QStringList() << "s" << "settings", "Specify settings INI file to skip startup dialog.", "file");
    parser.addOption(settingsOption);
    parser.process(app);

    QString settingsFileName = parser.value(settingsOption);

    app.setStyle(QStyleFactory::create("Fusion"));
    BoardSelectDialog boardSelectDialog(settingsFileName);
    return app.exec();
}
