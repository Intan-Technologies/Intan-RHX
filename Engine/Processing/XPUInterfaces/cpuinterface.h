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

#ifndef CPUINTERFACE_H
#define CPUINTERFACE_H

#include "abstractxpuinterface.h"

typedef struct _UnitDetection
{
    bool hoops[4];
} HoopDetectionStruct;

typedef struct _ChannelDetection
{
    HoopDetectionStruct units[4];
    bool maxSurpassed;
} ChannelDetectionStruct;

class CPUInterface : public AbstractXPUInterface
{
    Q_OBJECT
public:
    explicit CPUInterface(SystemState* state_, QObject *parent = nullptr);
    ~CPUInterface();

    void processDataBlock(uint16_t* data, uint16_t* lowChunk, uint16_t* wideChunk,
                          uint16_t* highChunk, uint32_t* spikeChunk, uint8_t* spikeIDChunk) override;
    void speedTest() override;
    bool setupMemory() override;
    bool cleanupMemory() override;

private:
    void initializeMemory();
    void freeMemory();
};

#endif // CPUINTERFACE_H
