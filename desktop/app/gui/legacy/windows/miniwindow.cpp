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

#include <QMenu>

TMiniWindow::TMiniWindow(QWidget *parent) : TAbstractWindow(parent)
  , mBtnPlay(new TImageButton(this))
  , mBtnPrev(new TImageButton(this))
  , mBtnNext(new TImageButton(this))
  , mBtnPause(new TImageButton(this))
  , mBtnStop(new TImageButton(this))
  , mBtnOpen(new TImageButton(this))
  , mBtnMute(new TImageButton(this))
  , mBtnMinimize(new TImageButton(this))
  , mBtnLyrics(new TImageButton(this))
  , mBtnExit(new TImageButton(this))
  , mBtnNormalMode(new TImageButton(this))
  , mProgressBar(new TSliderBar(Qt::Horizontal, this))
  , mVolumeBar(new TSliderBar(Qt::Horizontal, this))
  , mIcon(new TLabel(this))
  , mMusicTitle(new TScrollLabel(this))
  , mLedTime(new TLedWidget(this))
  , mStereo(new TLabel(this))
  , mStatus(new TLabel(this))
  , mVisualWidget(new TVisualWidget(this))
{
    connect(mBtnExit, SIGNAL(clicked()), this, SIGNAL(exitClicked()));
    connect(mBtnPlay, SIGNAL(clicked()), this, SIGNAL(playClicked()));
    connect(mBtnPause, SIGNAL(clicked()), this, SIGNAL(pauseClicked()));
    connect(mBtnOpen, SIGNAL(clicked()), this, SIGNAL(openMusicsClicked()));
    connect(mBtnStop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(mBtnNext, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(mBtnPrev, SIGNAL(clicked()), this, SIGNAL(prevClicked()));
    connect(mBtnMinimize, SIGNAL(clicked()), this, SIGNAL(requestShowMinimized()));
    connect(mBtnNormalMode, SIGNAL(clicked()), this, SIGNAL(normalModeClicked()));
    connect(mBtnLyrics, SIGNAL(clicked(bool)), this, SIGNAL(lyricButtonToggle(bool)));
    connect(mBtnMute, SIGNAL(clicked(bool)), this, SIGNAL(volumeToggle(bool)));
    connect(mVolumeBar, SIGNAL(valueChanged(int)), this, SIGNAL(volumeValueChanged(int)));
    connect(mProgressBar, SIGNAL(valueChanged(int)), this, SIGNAL(progressChanged(int)));
    connect(mIcon, SIGNAL(clicked()), this, SLOT(on_icoLogo_clicked()));

    mBtnLyrics->setVisible(false);
    retranslateUi();
}

TMiniWindow::~TMiniWindow()
{

}

void TMiniWindow::setContextMenu(QMenu *menu)
{
    mContextMenu = menu;
}

void TMiniWindow::retranslateUi()
{
    mBtnPause->setToolTip(tr("Pause(%1)").arg(mBtnPause->shortcut().toString()));
    mBtnPlay->setToolTip(tr("Play(%1)").arg(mBtnPlay->shortcut().toString()));
    mBtnNormalMode->setToolTip(tr("Normal Mode(%1)").arg(mBtnNormalMode->shortcut().toString()));
    mBtnOpen->setToolTip(tr("Open music(%1)").arg(mBtnOpen->shortcut().toString()));
    mBtnMute->setToolTip(tr("Volume switch(%1)").arg(mBtnMute->shortcut().toString()));
    mBtnPrev->setToolTip(tr("Previous(%1)").arg(mBtnPrev->shortcut().toString()));
    mBtnStop->setToolTip(tr("Stop(%1)").arg(mBtnStop->shortcut().toString()));
    mBtnExit->setToolTip(tr("Exit"));
    mBtnNext->setToolTip(tr("Next(%1)").arg(mBtnNext->shortcut().toString()));
    mBtnMinimize->setToolTip(tr("Minimize"));
    mBtnLyrics->setToolTip(tr("Lyric(%1)").arg(mBtnLyrics->shortcut().toString()));
    mIcon->setToolTip(tr("Main menu"));
    mProgressBar->setToolTip(tr("Progress"));
    mVolumeBar->setToolTip(tr("Volume: %1%").arg(mVolumeBar->value()));
}

void TMiniWindow::on_icoLogo_clicked()
{
    if(mContextMenu)
        mContextMenu->popup(QCursor::pos());
}

void TMiniWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(mContextMenu)
        mContextMenu->popup(event->globalPos());

    TAbstractWindow::contextMenuEvent(event);
}

void TMiniWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnPlay->loadFromSkin(element.firstChildElement(TAG_MINI_PLAY), skin);
    mBtnPause->loadFromSkin(element.firstChildElement(TAG_MINI_PAUSE), skin);
    mBtnPrev->loadFromSkin(element.firstChildElement(TAG_MINI_PREV), skin);
    mBtnNext->loadFromSkin(element.firstChildElement(TAG_MINI_NEXT), skin);
    mBtnStop->loadFromSkin(element.firstChildElement(TAG_MINI_STOP), skin);
    mBtnOpen->loadFromSkin(element.firstChildElement(TAG_MINI_OPEN), skin);
    mBtnMute->loadFromSkin(element.firstChildElement(TAG_MINI_MUTE), skin);
    mBtnLyrics->loadFromSkin(element.firstChildElement(TAG_MINI_LYRIC), skin);
    mBtnMinimize->loadFromSkin(element.firstChildElement(TAG_MINI_MINIMIZE), skin);
    mBtnNormalMode->loadFromSkin(element.firstChildElement(TAG_MINI_MINIMODE), skin);
    mBtnExit->loadFromSkin(element.firstChildElement(TAG_MINI_EXIT), skin);
    mLedTime->loadFromSkin(element.firstChildElement(TAG_MINI_LED), skin);

    update();
}
