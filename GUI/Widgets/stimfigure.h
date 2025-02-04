//------------------------------------------------------------------------------
//
//  Intan Technologies RHX Data Acquisition Software
//  Version 3.4.0
//
//  Copyright (c) 2020-2025 Intan Technologies
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

#ifndef STIMFIGURE_H
#define STIMFIGURE_H

#include <QWidget>
#include "abstractfigure.h"

class StimFigure : public AbstractFigure
{
    Q_OBJECT
public:
    explicit StimFigure(StimParameters* stimParameters, QWidget *parent = 0);

    void uniqueRedraw(QPainter &painter) override;

public slots:
    void updateEnableAmpSettle(bool enabled);
    void updateMaintainAmpSettle(bool maintain);
    void updateEnableChargeRecovery(bool enabled);
    void highlightSecondPhaseDuration(bool highlight);
    void highlightInterphaseDelay(bool highlight);
    void highlightFirstPhaseAmplitude(bool highlight);
    void highlightSecondPhaseAmplitude(bool highlight);
    void highlightPreStimAmpSettle(bool highlight);
    void highlightPostStimAmpSettle(bool highlight);
    void highlightPostStimChargeRecovOn(bool highlight);
    void highlightPostStimChargeRecovOff(bool highlight);

private:
    bool localEnableAmpSettle;
    bool localMaintainAmpSettle;
    bool localEnableChargeRecovery;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};

#endif // STIMFIGURE_H
