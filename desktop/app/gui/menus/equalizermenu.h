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
#ifndef TEQUALIZERMENU_H
#define TEQUALIZERMENU_H

#include "abstractmenu.h"

class TEqualizerMenu : public TAbstractMenu
{
    Q_OBJECT

public:
    TEqualizerMenu(QWidget *parent = 0);
    ~TEqualizerMenu();

    void loadSettings();
    bool isCustomizeActionChecked();
    void checkCustomizeAction();

signals:
    void onRockSelected();
    void onMetalSelected();
    void onElectricSelected();
    void onPopSelected();
    void onJazzSelected();
    void onClassicSelected();
    void onCustomizeSelected();

private slots:
    void slotActionTriggered(bool);

private:
    QAction *mActionRock;
    QAction *mActionMetal;
    QAction *mActionElectric;
    QAction *mActionPop;
    QAction *mActionJazz;
    QAction *mActionClassic;
    QAction *mActionCustomize;
    QAction *mLastActivedAction;

public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TEQUALIZERMENU_H
