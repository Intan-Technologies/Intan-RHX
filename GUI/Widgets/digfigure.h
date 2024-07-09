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

#ifndef DIGFIGURE_H
#define DIGFIGURE_H

#include <QWidget>
#include "stimparameters.h"
#include "abstractfigure.h"

class DigFigure : public AbstractFigure
{
    Q_OBJECT
public:
    explicit DigFigure(StimParameters* stimParameters, QWidget* parent = nullptr);
    void uniqueRedraw(QPainter &painter);

private:
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
};


#endif // DIGFIGURE_H
