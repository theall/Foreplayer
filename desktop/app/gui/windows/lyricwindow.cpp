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
#include "lyricwindow.h"

TLyricWindow::TLyricWindow(QWidget *parent) : TAbstractWindow(parent, true)
{
    setObjectName("LyricWindow");

    mBtnClose = new TImageButton(this);
    mBtnOnTop = new TImageButton(this);
    mBtnDesktopLyric = new TImageButton(this);
    mLyric = new TLabel(this);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));
    connect(mBtnOnTop, SIGNAL(clicked(bool)), this, SLOT(on_btnOnTop_clicked(bool)));
    connect(mBtnDesktopLyric, SIGNAL(clicked()), this, SLOT(on_btnShowDescktopLyric_clicked()));

    retranslateUi();
}

void TLyricWindow::setStringList(QStringList sl)
{
    mLyric->setText(sl.join("\n"));
}

void TLyricWindow::on_btnClose_clicked()
{
    hide();

    emit lyricWindowToggled(false);
}

void TLyricWindow::on_btnShowDescktopLyric_clicked()
{
    hide();
    emit requireShowDesktopLyric();
}

void TLyricWindow::on_btnOnTop_clicked(bool checked)
{
    Qt::WindowFlags flags = windowFlags();
    if(checked)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags ^= Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    emit lyricWindowOntopToggled(checked);
}

void TLyricWindow::retranslateUi()
{
    mBtnClose->setToolTip(tr("Close"));
    mBtnOnTop->setToolTip(tr("Always on top"));
    mBtnDesktopLyric->setToolTip(tr("Show desctop lyric"));
}

void TLyricWindow::resizeEvent(QResizeEvent *event)
{
    TAbstractWindow::resizeEvent(event);

    mBtnClose->updatePos();
    mBtnOnTop->updatePos();
    mBtnDesktopLyric->updatePos();
}

void TLyricWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnClose->loadFromSkin(element.firstChildElement(TAG_LYRIC_CLOSE), skin);
    mBtnOnTop->loadFromSkin(element.firstChildElement(TAG_LYRIC_ONTOP), skin);
    mBtnDesktopLyric->loadFromSkin(element.firstChildElement(TAG_LYRIC_DESKLRC), skin);

    update();
}
