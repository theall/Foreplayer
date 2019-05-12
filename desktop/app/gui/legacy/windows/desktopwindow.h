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
#ifndef TDESKTOPWINDOW_H
#define TDESKTOPWINDOW_H

#include "abstractwindow.h"

#include <QDesktopWidget>

class TDesktopWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TDesktopWindow(QWidget *parent = 0);
    ~TDesktopWindow();

private slots:
    void slotScreenResized(int);

private:
    int mScreen;
    QDesktopWidget *mDesktop;
    void updateGeometry();

    // TAbstractWindow interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TDESKTOPWINDOW_H
