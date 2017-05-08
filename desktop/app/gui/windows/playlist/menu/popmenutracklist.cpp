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
#include "popmenutracklist.h"

TPopMenuTrackList::TPopMenuTrackList(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionPlay = new QAction(this);
    mActionCopy = new QAction(this);
    mActionChange = new QAction(this);
    mActionExport = new QAction(this);
    mActionView = new QAction(this);

    connect(mActionPlay, SIGNAL(triggered()), this, SIGNAL(onActionPlayTriggered()));
    connect(mActionCopy, SIGNAL(triggered()), this, SIGNAL(onActionCopyTriggered()));
    connect(mActionChange, SIGNAL(triggered()), this, SIGNAL(onActionChangeTriggered()));
    connect(mActionExport, SIGNAL(triggered()), this, SIGNAL(onActionExportTriggered()));
    connect(mActionView, SIGNAL(triggered()), this, SIGNAL(onActionViewTriggered()));

    addAction(mActionPlay);
    addSeparator();
    //addAction(mActionCopy);
    //addSeparator();
    addAction(mActionChange);
    addSeparator();
    addAction(mActionExport);
    addSeparator();
    addAction(mActionView);

    retranslateUi();
}

void TPopMenuTrackList::display(QPoint pos, int selectionCount)
{
    bool hasSelection = selectionCount>0;
    bool singleSelection = selectionCount==1;

    mActionPlay->setEnabled(singleSelection);
    mActionChange->setEnabled(singleSelection);
    mActionView->setEnabled(singleSelection);
    mActionCopy->setEnabled(hasSelection);
    mActionExport->setEnabled(hasSelection);

    popup(pos);
}

void TPopMenuTrackList::retranslateUi()
{
    mActionPlay->setText(tr("Play"));
    mActionCopy->setText(tr("Copy"));
    mActionChange->setText(tr("Rename/Change"));
    mActionExport->setText(tr("Export"));
    mActionView->setText(tr("View property"));
}
