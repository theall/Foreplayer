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
#include "popmenuaddmusics.h"

TPopMenuAddMusics::TPopMenuAddMusics(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionAddMusics = addAction(QString(), this, SIGNAL(onActionAddMusicsTriggered()));
    mActionAddDirectory = addAction(QString(), this, SIGNAL(onActionAddDirectoryTriggered()));
    retranslateUi();
}

TPopMenuAddMusics::~TPopMenuAddMusics()
{

}

void TPopMenuAddMusics::retranslateUi()
{
    setTitle(tr("Add"));

    mActionAddMusics->setText(tr("Add musics"));
    mActionAddDirectory->setText(tr("Add music directory"));
}
