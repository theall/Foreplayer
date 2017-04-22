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
#include "mainmenu.h"

TMainMenu::TMainMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mPlayingMenu(new TPlayControlMenu(this)),
    mVolumeMenu(new TVolumeControlMenu(this)),
    mPlaymodeMenu(new TPlaymodeMenu(this)),
    mSkinMenu(new TSkinMenu(this)),
    mTransparentMenu(new TTransparentMenu(this)),
    mViewMenu(new QMenu(this)),
    mFullScreenMenu(new QMenu(this)),
    mAboutMenu(new QMenu(this)),
    mActionAbout(new QAction(this)),
    mActionAboutQt(new QAction(this)),
    mActionOptions(new QAction(this)),
    mActionExit(new QAction(this))
{
    connect(mActionAbout, SIGNAL(triggered()), this, SIGNAL(onAboutTriggered()));
    connect(mActionAboutQt, SIGNAL(triggered()), this, SIGNAL(onAboutQtTriggered()));
    connect(mActionOptions, SIGNAL(triggered()), this, SIGNAL(onOptionsTriggered()));
    connect(mActionExit, SIGNAL(triggered()), this, SIGNAL(onExitTriggered()));

    mAboutMenu->addAction(mActionAbout);
    mAboutMenu->addAction(mActionAboutQt);

    addMenu(mPlayingMenu);
    addMenu(mVolumeMenu);
    addMenu(mPlaymodeMenu);
    addSeparator();

    //addMenu(mViewMenu);
    //addSeparator();
    addMenu(mSkinMenu);
    addMenu(mTransparentMenu);
    //addMenu(mFullScreenMenu);
    addSeparator();
    addAction(mActionOptions);
    addSeparator();
    addMenu(mAboutMenu);
    addSeparator();
    addAction(mActionExit);

    retranslateUi();
}

TMainMenu::~TMainMenu()
{

}

TPlayControlMenu *TMainMenu::playControlMenu()
{
    return mPlayingMenu;
}

TVolumeControlMenu *TMainMenu::volumeControlMenu()
{
    return mVolumeMenu;
}

TPlaymodeMenu *TMainMenu::playmodeMenu()
{
    return mPlaymodeMenu;
}

TSkinMenu *TMainMenu::skinMenu()
{
    return mSkinMenu;
}

TTransparentMenu *TMainMenu::transparentMenu()
{
    return mTransparentMenu;
}

void TMainMenu::retranslateUi()
{
    mPlayingMenu->setTitle(tr("Play Control"));
    mVolumeMenu->setTitle(tr("Volume Control"));
    mPlaymodeMenu->setTitle(tr("Play Mode"));
    mViewMenu->setTitle(tr("View"));
    mSkinMenu->setTitle(tr("Skins"));
    mTransparentMenu->setTitle(tr("Transparent"));
    mFullScreenMenu->setTitle(tr("Fullscreen"));
    mAboutMenu->setTitle(tr("About"));
    mActionAbout->setText(tr("About Foreplayer"));
    mActionAboutQt->setText(tr("About Qt"));
    mActionOptions->setText(tr("Options"));
    mActionExit->setText(tr("Exit"));
}
