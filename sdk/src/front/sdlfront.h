/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef TSDLFRONT_H
#define TSDLFRONT_H

#include "abstractfront.h"

class TSDLFront : public TAbstractFront
{
public:
    explicit TSDLFront();
    ~TSDLFront();

    bool start();
    void stop();
    void step();

    void play();
    void pause();

    void setRecorder();
    void setFilter();

    void setSampleSize(int sampleSize);

private:
};

#endif // TSDLFRONT_H
