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
#include "popmenuremovemusics.h"

TPopMenuRemoveMusics::TPopMenuRemoveMusics(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionRemoveSelections = addAction(QString(), this, SIGNAL(onActionRemoveSelectionsTriggered()));
    mActionRemoveRedundant = addAction(QString(), this, SIGNAL(onActionRemoveRedundantTriggered()));
    mActionRemoveErrors = addAction(QString(), this, SIGNAL(onActionRemoveErrorsTriggered()));
    addSeparator();
    mActionRemoveAll = addAction(QString(), this, SIGNAL(onActionRemoveAllTriggered()));
    retranslateUi();
}

TPopMenuRemoveMusics::~TPopMenuRemoveMusics()
{

}

void TPopMenuRemoveMusics::retranslateUi()
{
    setTitle(tr("Remove"));

    mActionRemoveSelections->setText(tr("Selections"));
    mActionRemoveRedundant->setText(tr("Redundant"));
    mActionRemoveErrors->setText(tr("Errors"));
    mActionRemoveAll->setText(tr("All"));
}
