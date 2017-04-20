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
#ifndef TDESKTOPLYRICWINDOW_H
#define TDESKTOPLYRICWINDOW_H

#include "widgets/imagebutton.h"
#include "abstractwindow.h"

class TDesktopLyricWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TDesktopLyricWindow(QWidget *parent = 0);

signals:
    void requireShowLyricWindow();
    void lyricWindowToggled(bool);
    void lyricWindowOntopToggled(bool);

public slots:

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_btnClose_clicked();
    void on_btnOnTop_clicked(bool checked);
    void on_btnBack_clicked();

private:
    TImageButton *mBtnIcon;
    TImageButton *mBtnPlay;
    TImageButton *mBtnPause;
    TImageButton *mBtnPrev;
    TImageButton *mBtnNext;
    TImageButton *mBtnList;
    TImageButton *mBtnSettings;
    TImageButton *mBtnKalaok;
    TImageButton *mBtnLines;
    TImageButton *mBtnLock;
    TImageButton *mBtnOnTop;
    TImageButton *mBtnBack;
    TImageButton *mBtnClose;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TDESKTOPLYRICWINDOW_H
