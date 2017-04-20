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
#ifndef TTOOLBAR_H
#define TTOOLBAR_H

#include "imagebutton.h"

#include "../../share/skin.h"

class TToolBar : public QWidget, TSkinReader
{
    Q_OBJECT

public:
    enum BUTTON
    {
        BTN_ADD,
        BTN_DELETE,
        BTN_PLAYLIST,
        BTN_SORT,
        BTN_FIND,
        BTN_EDIT,
        BTN_MODE,
        BUTTON_COUNT
    };

    TToolBar(QWidget *parent=0);
    ~TToolBar();

    void setPixmaps(QPixmap normal, QPixmap hover, QColor transparentColor);
    Qt::Alignment alignment();
    void setAlignment(QPixmap pixmap, Qt::Alignment alignment);

    void uncheckButtons();
    void retranslateUi();
    void updatePos();

signals:
    void requestToggleContexMenu(TToolBar::BUTTON id, QPoint pos, bool checked);

private slots:
    void slotButtonClicked(bool);

private:
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TImageButton *mButtons[BUTTON_COUNT];

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TTOOLBAR_H
