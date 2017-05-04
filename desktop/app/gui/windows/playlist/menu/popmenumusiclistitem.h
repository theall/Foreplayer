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
#ifndef TPOPMENUMUSICLISTITEM_H
#define TPOPMENUMUSICLISTITEM_H

#include "abstractpopmenu.h"

class TPopMenuMusiclistItem : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuMusiclistItem(QWidget *parent = 0);
    ~TPopMenuMusiclistItem();

    void display(QPoint pos, int selectionCount);

signals:
    void onActionReparseTriggered();
    void onActionPlayTriggered();
    void onActionCopyTriggered();
    void onActionCutTriggered();
    void onActionPasteTriggered();
    void onActionDeleteTriggered();
    void onActionRenameTriggered();
    void onActionExplorerTriggered();
    void onActionExportTriggered();
    void onActionViewTriggered();
    void onActionCopyToUsbTriggered();

private:
    QAction *mActionCopy;
    QAction *mActionCopyToUsb;
    QAction *mActionCut;
    QAction *mActionRemove;
    QAction *mActionDetail;
    QAction *mActionExplorer;
    QAction *mActionExport;
    QAction *mActionPaste;
    QAction *mActionPlay;
    QAction *mActionRename;
    QAction *mActionReparse;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUMUSICLISTITEM_H
