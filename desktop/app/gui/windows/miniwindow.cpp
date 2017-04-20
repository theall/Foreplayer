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
#include "miniwindow.h"

TMiniWindow::TMiniWindow(QWidget *parent) : TAbstractWindow(parent)
  , mBtnPlay(new TImageButton(this))
  , mBtnPrev(new TImageButton(this))
  , mBtnNext(new TImageButton(this))
  , mBtnPause(new TImageButton(this))
  , mBtnStop(new TImageButton(this))
  , mBtnOpen(new TImageButton(this))
  , mBtnMute(new TImageButton(this))
  , mBtnMinimize(new TImageButton(this))
  , mBtnBrowser(new TImageButton(this))
  , mBtnEqualizer(new TImageButton(this))
  , mBtnPlaylist(new TImageButton(this))
  , mBtnLyrics(new TImageButton(this))
  , mBtnExit(new TImageButton(this))
  , mBtnMinimode(new TImageButton(this))
{

}

TMiniWindow::~TMiniWindow()
{

}

void TMiniWindow::retranslateUi()
{
    mBtnMinimode->setToolTip(tr("Mini Mode(%1)").arg(mBtnMinimode->shortcut().toString()));
}

void TMiniWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnPlay->loadFromSkin(element.firstChildElement(TAG_PLAYER_PLAY), skin);
    mBtnPause->loadFromSkin(element.firstChildElement(TAG_PLAYER_PAUSE), skin);
    mBtnPrev->loadFromSkin(element.firstChildElement(TAG_PLAYER_PREV), skin);
    mBtnNext->loadFromSkin(element.firstChildElement(TAG_PLAYER_NEXT), skin);
    mBtnStop->loadFromSkin(element.firstChildElement(TAG_PLAYER_STOP), skin);
    mBtnOpen->loadFromSkin(element.firstChildElement(TAG_PLAYER_OPEN), skin);
    mBtnMute->loadFromSkin(element.firstChildElement(TAG_PLAYER_MUTE), skin);
    mBtnLyrics->loadFromSkin(element.firstChildElement(TAG_PLAYER_LYRIC), skin);
    mBtnEqualizer->loadFromSkin(element.firstChildElement(TAG_PLAYER_EQUALIZER), skin);
    mBtnPlaylist->loadFromSkin(element.firstChildElement(TAG_PLAYER_PLAYLIST), skin);
    mBtnBrowser->loadFromSkin(element.firstChildElement(TAG_PLAYER_BROWSER), skin);
    mBtnMinimize->loadFromSkin(element.firstChildElement(TAG_PLAYER_MINIMIZE), skin);
    mBtnMinimode->loadFromSkin(element.firstChildElement(TAG_PLAYER_MINIMODE), skin);
    mBtnExit->loadFromSkin(element.firstChildElement(TAG_PLAYER_EXIT), skin);

    update();
}
