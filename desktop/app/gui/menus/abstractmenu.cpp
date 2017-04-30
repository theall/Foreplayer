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
#include "abstractmenu.h"

TAbstractMenu::TAbstractMenu(QWidget *parent, bool keepActive) :
    QMenu(parent)
  , mKeepActive(keepActive)
{

}

TAbstractMenu::~TAbstractMenu()
{

}

bool TAbstractMenu::event(QEvent *event)
{
    if(event->type()==QEvent::LanguageChange)
    {
        retranslateUi();
    }
    return QMenu::event(event);
}

void TAbstractMenu::closeEvent(QCloseEvent *ev)
{
    if(mKeepActive)
    {
        QObjectList objects = children();
        bool hasChildMenuVisible = false;
        for(QObject *object : objects)
        {
            QMenu *menu = qobject_cast<QMenu*>(object);
            if(menu && menu->isVisible())
            {
                hasChildMenuVisible = true;
                break;
            }
        }
        if(hasChildMenuVisible || rect().contains(mapFromGlobal(QCursor::pos())))
            ev->ignore();
        else
            ev->accept();
    } else {
        ev->accept();
    }
}
