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

#include "statusbars.h"

StatusBars::StatusBars(QWidget *parent) :
    QWidget(parent)
{
    hwBufferPercent = 0.0;
    swBufferPercent = 0.0;
    cpuLoadPercent = 0.0;
    hwMinorWarningCount = 0;
    hwMajorWarningCount = 0;
    swMinorWarningCount = 0;
    swMajorWarningCount = 0;

    background = QImage(":images/status_header.png");
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumSize(background.size());
    setMaximumSize(background.size());
    image = QImage(background.size(), QImage::Format_ARGB32_Premultiplied);
}

QSize StatusBars::minimumSizeHint() const
{
    return background.size();
}

QSize StatusBars::sizeHint() const
{
    return background.size();
}

void StatusBars::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(&image);
    painter.drawImage(QPoint(0, 0), background);

    painter.fillRect(QRect(48, 3, round(hwBufferPercent), 5), colorFromPercent(hwBufferPercent));
    painter.fillRect(QRect(48, 11, round(swBufferPercent), 5), colorFromPercent(swBufferPercent));
    painter.fillRect(QRect(48, 19, round(cpuLoadPercent), 5), colorFromPercent(cpuLoadPercent));

    QStylePainter stylePainter(this);
    stylePainter.drawImage(0, 0, image);
}

void StatusBars::updateBars(double hwBufferPercent_, double swBufferPercent_, double cpuLoadPercent_)
{
    // Emit signal for changes in hw buffer warning status
    if (hwBufferPercent_ >= MajorWarningThreshold) {
        if (hwBufferPercent < MajorWarningThreshold) {
            hwMajorWarningCount = 1; // When entering MajorWarning zone from below, start listening for two more instances above the threshold
        } else if (hwMajorWarningCount > 0) {
            hwMajorWarningCount++; // When listening above the threshold, keep track of how many instances in a row stay there
        }

        if (hwMajorWarningCount > 3) {
            emit bufferStatusChanged(BufferMajorWarning); // When 3 instances in a row above major threshold occur, emit MajorWarning signal
            hwMajorWarningCount = 0;
        }
    }

    else if (hwBufferPercent_ >= MinorWarningThreshold) {
        if (hwBufferPercent < MinorWarningThreshold) {
            hwMinorWarningCount = 1; // When entering MinorWarning zone from below, start listening for two more instances above the threshold
        } else if (hwMinorWarningCount > 0) {
            hwMinorWarningCount++; // When listening above the threshold, keep track of how many instances in a row stay there
        }

        if (hwMinorWarningCount > 3) {
            emit bufferStatusChanged(BufferMinorWarning); // When 3 instances in a row above major threhsold occur, emit MinorWarning signal
            hwMinorWarningCount = 0;
        }

        if (hwBufferPercent >= MajorWarningThreshold) {
            hwMajorWarningCount = 0; // When entering MinorWarning zone from above, reset majorWarningCount to 0
        }
    }

    else {
        if ((hwBufferPercent_ < MinorWarningThreshold) && (hwBufferPercent >= MinorWarningThreshold)) {
            emit bufferStatusChanged(BufferNoWarning); // When entering NoWarning zone from above, emit NoWarning signal
            hwMajorWarningCount = 0; // Reset majorWarningCount to 0
            hwMinorWarningCount = 0; // Reset minorWarningCount to 0
        }
    }

    // Emit signal for changes in sw buffer warning status
    if (swBufferPercent_ >= MajorWarningThreshold) {
        if (swBufferPercent < MajorWarningThreshold) {
            swMajorWarningCount = 1; // When entering MajorWarning zone from below, start listening for two more instances above the threshold
        } else if (swMajorWarningCount > 0) {
            swMajorWarningCount++; // When listening above the threshold, keep track of how many instances in a row stay there
        }

        if (swMajorWarningCount > 3) {
            emit bufferStatusChanged(BufferMajorWarning); // When 3 instances in a row above major threshold occur, emit MajorWarning signal
            swMajorWarningCount = 0;
        }
    }

    else if (swBufferPercent_ >= MinorWarningThreshold) {
        if (swBufferPercent < MinorWarningThreshold) {
            swMinorWarningCount = 1; // When entering MinorWarning zone from below, start listening for two more instances above the threshold
        } else if (swMinorWarningCount > 0) {
            swMinorWarningCount++; // When listening above the threshold, keep track of how many instances in a row stay there
        }

        if (swMinorWarningCount > 3) {
            emit bufferStatusChanged(BufferMinorWarning); // When 3 instances in a row above major threhsold occur, emit MinorWarning signal
            swMinorWarningCount = 0;
        }

        if (swBufferPercent >= MajorWarningThreshold) {
            swMajorWarningCount = 0; // When entering MinorWarning zone from above, reset majorWarningCount to 0
        }
    }

    else {
        if ((swBufferPercent_ < MinorWarningThreshold) && (swBufferPercent >= MinorWarningThreshold)) {
            emit bufferStatusChanged(BufferNoWarning); // When entering NoWarning zone from above, emit NoWarning signal
            swMajorWarningCount = 0; // Reset majorWarningCount to 0
            swMinorWarningCount = 0; // Reset minorWarningCount to 0
        }
    }

    hwBufferPercent = hwBufferPercent_;
    swBufferPercent = swBufferPercent_;
    cpuLoadPercent = cpuLoadPercent_;
    update();
}

QColor StatusBars::colorFromPercent(double percent) const
{
    if (percent >= MajorWarningThreshold) return QColor(245, 10, 0); // red
    else if (percent >= MinorWarningThreshold) return QColor(255, 191, 0); // amber
    else return QColor(0, 150, 0); // green
}
