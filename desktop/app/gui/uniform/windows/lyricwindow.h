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
#ifndef TLYRICWINDOW_H
#define TLYRICWINDOW_H

#include "../widgets/imagebutton.h"
#include "../widgets/label.h"
#include "abstractwindow.h"

class TLyricWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TLyricWindow(QWidget *parent = 0);
    ~TLyricWindow();

    void setStringList(QStringList sl);

signals:
    void requireShowDesktopLyric();
    void lyricWindowToggled(bool);
    void lyricWindowOntopToggled(bool);

public slots:

private slots:
    void on_btnClose_clicked();
    void on_btnOnTop_clicked(bool checked);
    void on_btnShowDescktopLyric_clicked();

private:
    TLabel *mLyric;
    TImageButton *mBtnClose;
    TImageButton *mBtnOnTop;
    TImageButton *mBtnDesktopLyric;

    // QWidget interface
protected:
    void retranslateUi() Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TLYRICWINDOW_H
