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
#include "popmenumusiclistedit.h"

TPopMenuMusiclistEdit::TPopMenuMusiclistEdit(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionCopy = addAction(QString(), this, SIGNAL(onActionCopyTriggered()));
    mActionPaste = addAction(QString(), this, SIGNAL(onActionPasteTriggered()));
    mActionRemove = addAction(QString(), this, SIGNAL(onActionRemoveTriggered()));
    addSeparator();
    mActionSelectAll = addAction(QString(), this, SIGNAL(onActionSelectAllTriggered()));
    mActionUnSelectAll = addAction(QString(), this, SIGNAL(onActionUnSelectAllTriggered()));
    mActionInverse = addAction(QString(), this, SIGNAL(onActionInverseTriggered()));
    retranslateUi();
}

TPopMenuMusiclistEdit::~TPopMenuMusiclistEdit()
{

}

void TPopMenuMusiclistEdit::retranslateUi()
{
    setTitle(tr("Edit"));
    mActionCopy->setText(tr("Copy"));
    mActionPaste->setText(tr("Paste"));
    mActionRemove->setText(tr("Remove"));
    mActionSelectAll->setText(tr("SelectAll"));
    mActionUnSelectAll->setText(tr("UnSelect all"));
    mActionInverse->setText(tr("Inverse"));
}
