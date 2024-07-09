//------------------------------------------------------------------------------
//
//  Intan Technologies RHX Data Acquisition Software
//  Version 3.3.2
//
//  Copyright (c) 2020-2024 Intan Technologies
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
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  This software is provided 'as-is', without any express or implied warranty.
//  In no event will the authors be held liable for any damages arising from
//  the use of this software.
//
//  See <http://www.intantech.com> for documentation and product information.
//
//------------------------------------------------------------------------------

#ifndef DEMODIALOG_H
#define DEMODIALOG_H

#include <QDialog>
#include <QtWidgets>

#include "rhxglobals.h"


class DemoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DemoDialog(DemoSelections *demoSelection_, bool &useOpenCL_, uint8_t &playbackPorts_, QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *) override;

private:
    DemoSelections *demoSelection;

    bool *useOpenCL;
    uint8_t *playbackPorts;
    QLabel *message;
    QPushButton *usbInterfaceButton;
    QPushButton *recordControllerButton;
    QPushButton *stimControllerButton;
    QPushButton *playbackButton;
    QPushButton *advancedButton;

private slots:
    void usbInterface();
    void recordController();
    void stimController();
    void playback();
    void advanced();
};

#endif // DEMODIALOG_H
