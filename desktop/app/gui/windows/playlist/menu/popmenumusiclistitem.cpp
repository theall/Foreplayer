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
#include "popmenumusiclistitem.h"

TPopMenuMusiclistItem::TPopMenuMusiclistItem(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionReparse = new QAction(this);
    mActionPlay = new QAction(this);
    mActionCopy = new QAction(this);
    mActionCut = new QAction(this);
    mActionPaste = new QAction(this);
    mActionRemove = new QAction(this);
    mActionRename = new QAction(this);
    mActionExplorer = new QAction(this);
    mActionExport = new QAction(this);
    mActionDetail = new QAction(this);
    mActionCopyToUsb = new QAction(this);

    connect(mActionReparse, SIGNAL(triggered()), this, SIGNAL(onActionReparseTriggered()));
    connect(mActionPlay, SIGNAL(triggered()), this, SIGNAL(onActionPlayTriggered()));
    connect(mActionCopy, SIGNAL(triggered()), this, SIGNAL(onActionCopyTriggered()));
    connect(mActionCut, SIGNAL(triggered()), this, SIGNAL(onActionCutTriggered()));
    connect(mActionPaste, SIGNAL(triggered()), this, SIGNAL(onActionPasteTriggered()));
    connect(mActionRemove, SIGNAL(triggered()), this, SIGNAL(onActionDeleteTriggered()));
    connect(mActionRename, SIGNAL(triggered()), this, SIGNAL(onActionRenameTriggered()));
    connect(mActionExplorer, SIGNAL(triggered()), this, SIGNAL(onActionExplorerTriggered()));
    connect(mActionExport, SIGNAL(triggered()), this, SIGNAL(onActionExportTriggered()));
    connect(mActionDetail, SIGNAL(triggered()), this, SIGNAL(onActionViewTriggered()));
    connect(mActionCopyToUsb, SIGNAL(triggered()), this, SIGNAL(onActionCopyToUsbTriggered()));

#ifndef QT_DEBUG
    addAction(mActionReparse);
    addAction(mActionCopyToUsb);
#endif
    addAction(mActionPlay);
    addSeparator();
    addAction(mActionCopy);
    addAction(mActionCut);
    addAction(mActionPaste);
    addSeparator();
    addAction(mActionRemove);
    addSeparator();
    addAction(mActionRename);
    addSeparator();
    addAction(mActionExplorer);
    addAction(mActionExport);
    addSeparator();
    addAction(mActionDetail);

    retranslateUi();
}

TPopMenuMusiclistItem::~TPopMenuMusiclistItem()
{

}

void TPopMenuMusiclistItem::retranslateUi()
{
    mActionReparse->setText(tr("Reparse"));
    mActionCut->setText(tr("Cut"));
    mActionCopy->setText(tr("Copy"));
    mActionPlay->setText(tr("Play"));
    mActionPaste->setText(tr("Paste"));
    mActionRemove->setText(tr("Remove"));
    mActionExport->setText(tr("Export"));
    mActionExplorer->setText(tr("Explorer"));
    mActionRename->setText(tr("Rename title"));
    mActionDetail->setText(tr("View details"));
    mActionCopyToUsb->setText(tr("Copy to storage"));
}
