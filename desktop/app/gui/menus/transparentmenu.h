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
#ifndef TTRANSPARENTMENU_H
#define TTRANSPARENTMENU_H

#include "abstractmenu.h"

class TTransparentMenu : public TAbstractMenu
{
    Q_OBJECT
public:
    TTransparentMenu(QWidget *parent = 0);
    ~TTransparentMenu();

    void loadSettings();

signals:
    void onOpacityChanged(qreal);

private slots:
    void slotActionTriggered(bool);

private:
    QAction *mLastActivedAction;
    QAction *mActionDisable;
    QAction *mActionTransparent1;
    QAction *mActionTransparent2;
    QAction *mActionTransparent3;
    QAction *mActionTransparent4;
    QAction *mActionTransparent5;
    QAction *mActionTransparent6;
    QAction *mActionTransparent7;
    QAction *mActionTransparent8;
    QAction *mActionTransparent9;
    QAction *mActionDisableWhileActived;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TTRANSPARENTMENU_H
