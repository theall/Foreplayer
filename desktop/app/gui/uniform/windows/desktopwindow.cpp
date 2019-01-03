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
#include "desktopwindow.h"

TDesktopWindow::TDesktopWindow(QWidget *parent) :
    TAbstractWindow(parent),
    mDesktop(QApplication::desktop())
{
    setObjectName("DesktopWindow");

    mScreen = mDesktop->primaryScreen();
    connect(mDesktop, SIGNAL(resized(int)), this, SLOT(slotScreenResized(int)));

    updateGeometry();
}

TDesktopWindow::~TDesktopWindow()
{
    delete mDesktop;
}

void TDesktopWindow::slotScreenResized(int screen)
{
    if(mScreen==screen)
    {
        updateGeometry();
    }
}

void TDesktopWindow::updateGeometry()
{
    setGeometry(mDesktop->availableGeometry(mScreen));
}


void TDesktopWindow::retranslateUi()
{
}
