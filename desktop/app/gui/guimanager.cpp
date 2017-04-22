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
#include "guimanager.h"

#include <QMessageBox>
#include "app/utils/preferences.h"
#include "share/skinsetting.h"

void findGumedWindows(TWindowList &list, TWindowList &findedList)
{
    if(list.size()==0 || findedList.size()==0)
        return;

    TWindowList newList = list;
    TWindowList newFindedList = findedList;

    // First
    for(auto findedWnd : findedList)
    {
        for(auto checkingWnd : list)
        {
            if(checkingWnd->isGumTo(findedWnd))
            {
                newList.removeOne(checkingWnd);
                if(!newFindedList.contains(checkingWnd))
                    newFindedList.append(checkingWnd);
            }
        }
    }

    if(newList.size()==list.size() && newFindedList==findedList)
    {
        return;
    } else {
        list = newList;
        findedList = newFindedList;
        findGumedWindows(list, findedList);
    }
}

TGuiManager::TGuiManager(QObject *parent) : QObject(parent)
  , mMainWindow(new TMainWindow)
  , mMiniWindow(new TMiniWindow)
  , mLyricWindow(new TLyricWindow)
  , mEqualizerWindow(new TEqualizerWindow)
  , mPlaylistWindow(new TPlaylistWindow)
  , mBrowserWindow(new TBrowserWindow)
  , mDesktopLyricWindow(new TDesktopLyricWindow)
  , mDesktopWindow(new TDesktopWindow)
  , mPropertyDialog(new TPropertyDialog)
  , mExportDialog(new TExportDialog)
  , mExportMissionDialog(new TExportMissionsDialog)
  , mOptionDialog(new TOptionsDialog)
  , mMainMenu(new TMainMenu)
  , mTrayIcon(new QSystemTrayIcon)
  , mSkinManager(new TSkinManager)
  , mMinimode(false)
  , mShowDesktopLyric(false)
{
    qApp->setStyle("Fusion");
    mMainWindow->setContextMenu(mMainMenu);

    // Main window
    connect(mMainWindow, SIGNAL(exitClicked()), this, SLOT(slotRequestExit()));
    connect(mMainWindow, SIGNAL(lyricButtonToggle(bool)), this, SLOT(slotLyricButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(equalizerButtonToggle(bool)), this, SLOT(slotEqualizerButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(playlistButtonToggle(bool)), this, SLOT(slotPlaylistButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(browserButtonToggle(bool)), this, SLOT(slotBrowserButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));
    connect(mMainWindow, SIGNAL(requestShowMinimized()), this, SLOT(slotShowMinimized()));
    connect(mMainWindow, SIGNAL(onActivationChange()), this, SLOT(slotMainWindowActivationChanged()));
    connect(mMainWindow, SIGNAL(volumeToggle(bool)), this, SLOT(slotVolumeToggled(bool)));

    // Mini window
    connect(mMiniWindow, SIGNAL(exitClicked()), this, SLOT(slotRequestExit()));
    connect(mMiniWindow, SIGNAL(lyricButtonToggle(bool)), this, SLOT(slotLyricButtonToggled(bool)));
    connect(mMiniWindow, SIGNAL(equalizerButtonToggle(bool)), this, SLOT(slotEqualizerButtonToggled(bool)));
    connect(mMiniWindow, SIGNAL(playlistButtonToggle(bool)), this, SLOT(slotPlaylistButtonToggled(bool)));
    connect(mMiniWindow, SIGNAL(browserButtonToggle(bool)), this, SLOT(slotBrowserButtonToggled(bool)));
    connect(mMiniWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));
    connect(mMiniWindow, SIGNAL(requestShowMinimized()), this, SLOT(slotShowMinimized()));

    //Lyric window
    connect(mLyricWindow, SIGNAL(lyricWindowToggled(bool)), this, SLOT(slotLyricWindowToggled(bool)));
    connect(mLyricWindow, SIGNAL(requireShowDesktopLyric()), this, SLOT(slotRequireShowDesktopLyric()));
    connect(mLyricWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));
    connect(mLyricWindow, SIGNAL(requestResizeWindow(QRect)), this, SLOT(slotRequestResizeWindow(QRect)));

    //Equalizer window
    connect(mEqualizerWindow, SIGNAL(eqWindowToggled(bool)), this, SLOT(slotEqualizerWindowToggled(bool)));
    connect(mEqualizerWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));

    //Playlist window
    connect(mPlaylistWindow, SIGNAL(playlistWindowToggled(bool)), this, SLOT(slotPlaylistWindowToggled(bool)));
    connect(mPlaylistWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));
    connect(mPlaylistWindow, SIGNAL(requestResizeWindow(QRect)), this, SLOT(slotRequestResizeWindow(QRect)));

    //Browser window
    connect(mBrowserWindow, SIGNAL(browserWindowToggled(bool)), this, SLOT(slotBrowserWindowToggled(bool)));
    connect(mBrowserWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));

    //Desktop lyric window
    connect(mDesktopLyricWindow, SIGNAL(requireShowLyricWindow()), this, SLOT(slotRequireShowLyricWindow()));
    connect(mDesktopLyricWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));

    // Main menu
    connect(mMainMenu, SIGNAL(onExitTriggered()), this, SLOT(slotRequestExit()));
    connect(mMainMenu, SIGNAL(onOptionsTriggered()), this, SLOT(slotOpenOptionsDialog()));
    connect(mMainMenu, SIGNAL(onAboutTriggered()), this, SLOT(slotAbout()));
    connect(mMainMenu, SIGNAL(onAboutQtTriggered()), this, SLOT(slotAboutQt()));
    connect(mMainMenu->volumeControlMenu(), SIGNAL(onVolumeMuteToggled(bool)), this, SLOT(slotVolumeToggled(bool)));
    connect(mMainMenu->volumeControlMenu(), SIGNAL(onVolumeDownTriggered()), this, SLOT(slotVolumeDownTriggered()));
    connect(mMainMenu->volumeControlMenu(), SIGNAL(onVolumeUpTriggered()), this, SLOT(slotVolumeUpTriggered()));
    connect(mMainMenu->skinMenu(), SIGNAL(requestLoadSkin(int)), this, SLOT(slotRequestLoadSkin(int)));
    connect(mMainMenu->skinMenu(), SIGNAL(requestSkinNames(QStringList&)), this, SLOT(slotRequestSkinNames(QStringList&)));
    connect(mMainMenu->transparentMenu(), SIGNAL(onOpacityChanged(qreal)), this, SLOT(slotOnOpacityChanged(qreal)));

    // Tray icon
    mTrayIcon->setContextMenu(mMainMenu);
    connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    // Options dialog
    connect(mOptionDialog, SIGNAL(displayTrayIconToggled(bool)), this, SLOT(slotDisplayTrayIconToggled(bool)));
}

TGuiManager::~TGuiManager()
{
    if(mMainWindow)
    {
         delete mMainWindow;
         mMainWindow = NULL;
    }
    if(mMiniWindow)
    {
         delete mMiniWindow;
         mMiniWindow = NULL;
    }
    if(mLyricWindow)
    {
         delete mLyricWindow;
         mLyricWindow = NULL;
    }
    if(mEqualizerWindow)
    {
         delete mEqualizerWindow;
         mEqualizerWindow = NULL;
    }
    if(mPlaylistWindow)
    {
         delete mPlaylistWindow;
         mPlaylistWindow = NULL;
    }
    if(mBrowserWindow)
    {
         delete mBrowserWindow;
         mBrowserWindow = NULL;
    }
    if(mDesktopLyricWindow)
    {
         delete mDesktopLyricWindow;
         mDesktopLyricWindow = NULL;
    }
    if(mDesktopWindow)
    {
         delete mDesktopWindow;
         mDesktopWindow = NULL;
    }
    if(mPropertyDialog)
    {
         delete mPropertyDialog;
         mPropertyDialog = NULL;
    }
    if(mExportDialog)
    {
         delete mExportDialog;
         mExportDialog = NULL;
    }
    if(mExportMissionDialog)
    {
         delete mExportMissionDialog;
         mExportMissionDialog = NULL;
    }
    if(mOptionDialog)
    {
         delete mOptionDialog;
         mOptionDialog = NULL;
    }
    if(mMainMenu)
    {
         delete mMainMenu;
         mMainMenu = NULL;
    }
    if(mTrayIcon)
    {
         delete mTrayIcon;
         mTrayIcon = NULL;
    }
    if(mSkinManager)
    {
        delete mSkinManager;
        mSkinManager = NULL;
    }
}

bool TGuiManager::loadSkin(QString fileName)
{
    if(fileName.isEmpty())
        return false;

    TSkin skin;

    bool result = skin.load(fileName);
    if(result)
    {
        loadSkin(&skin);
    } else {
        QMessageBox::critical(
            mMainWindow,
            tr("Load skin error."),
            skin.lastError());
    }

    return result;
}

bool TGuiManager::tryLoadSkins()
{
    bool ret = false;
    QString skinPath = TPreferences::instance()->skinPath();
    if(!skinPath.isEmpty())
        ret = loadSkin(skinPath);
    else {
        if(mSkinManager)
        {
            for(int i=0;i<mSkinManager->size();i++)
            {
                TSkin *skin = mSkinManager->skinAt(i);
                if(loadSkin(skin))
                {
                    ret = true;
                    break;
                }
            }
        }
    }

    return ret;
}

void TGuiManager::open()
{
    TPreferences *prefs = TPreferences::instance();
    QByteArray g, s;
    prefs->windowGeometryState(WT_MAIN, &g, &s);
    mMainWindow->restoreGeometry(g);
    mMainWindow->restoreState(s);

    prefs->windowGeometryState(WT_LYRIC, &g, &s);
    mLyricWindow->restoreGeometry(g);
    mLyricWindow->restoreState(s);

    prefs->windowGeometryState(WT_EQUALIZER, &g, &s);
    mEqualizerWindow->restoreGeometry(g);
    mEqualizerWindow->restoreState(s);

    prefs->windowGeometryState(WT_PLAYLIST, &g, &s);
    mPlaylistWindow->restoreGeometry(g);
    mPlaylistWindow->restoreState(s);

    mMainWindow->show();

    mEqualizerWindow->loadSettings();
    mMainWindow->loadSettings();

    // Load settings
    slotVolumeToggled(prefs->muteEnabled());

//    if(prefs->lyricWindowVisible())
//    {
//        mMainWindow->checkLyricButton(true);
//        //mLyricWindow->show();
//    }
//    if(prefs->eqWindowVisible())
//    {
//        mMainWindow->checkEqualizerButton(true);
//        mEqualizerWindow->show();
//    }
//    if(prefs->playlistWindowVisible())
//    {
//        mMainWindow->checkPlaylistButton(true);
//        mPlaylistWindow->show();
//    }
    //mBrowserWindow->show();

    slotVolumeToggled(prefs->muteEnabled());

    if(prefs->displayTrayIcon())
        mTrayIcon->show();

    mMainMenu->transparentMenu()->loadSettings();
}

void TGuiManager::close()
{
    mMainWindow->close();
}

void TGuiManager::setCaption(QString caption)
{
    QString newCaption = tr("%1 - %2").arg(caption).arg(QApplication::applicationName());
    mMainWindow->setCaption(newCaption);
    mTrayIcon->setToolTip(newCaption);
}

void TGuiManager::slotLyricWindowToggled(bool toggled)
{
    mMainWindow->checkLyricButton(toggled);
}

void TGuiManager::slotLyricButtonToggled(bool toggled)
{
    if(toggled)
        mLyricWindow->show();
    else
        mLyricWindow->hide();
}

void TGuiManager::slotEqualizerWindowToggled(bool toggled)
{
    mMainWindow->checkEqualizerButton(toggled);
}

void TGuiManager::slotEqualizerButtonToggled(bool toggled)
{
    if(toggled)
        mEqualizerWindow->show();
    else
        mEqualizerWindow->hide();
}

void TGuiManager::slotPlaylistWindowToggled(bool toggled)
{
    mMainWindow->checkPlaylistButton(toggled);
}

void TGuiManager::slotPlaylistButtonToggled(bool toggled)
{
    if(toggled)
        mPlaylistWindow->show();
    else
        mPlaylistWindow->hide();
}

void TGuiManager::slotBrowserWindowToggled(bool toggled)
{
    mMainWindow->checkBrowserButton(toggled);
}

void TGuiManager::slotBrowserButtonToggled(bool toggled)
{
    if(toggled)
        mBrowserWindow->show();
    else
        mBrowserWindow->hide();
}

void TGuiManager::slotRequestMoveWindow(QPoint newPos)
{
    TAbstractWindow *window = static_cast<TAbstractWindow *>(sender());
    if(window)
        moveWindow(window, newPos.x(), newPos.y());
}

void TGuiManager::slotRequestResizeWindow(QRect newGeometry)
{
    TAbstractWindow *window = static_cast<TAbstractWindow *>(sender());
    if(window)
    {
        TWindowList checkingWindows;
        if(mMainWindow->isVisible())
            checkingWindows.append(mMainWindow);
        if(mLyricWindow->isVisible())
            checkingWindows.append(mLyricWindow);
        if(mEqualizerWindow->isVisible())
            checkingWindows.append(mEqualizerWindow);
        if(mPlaylistWindow->isVisible())
            checkingWindows.append(mPlaylistWindow);
        if(mBrowserWindow->isVisible())
            checkingWindows.append(mBrowserWindow);
        if(mDesktopLyricWindow->isVisible())
            checkingWindows.append(mDesktopLyricWindow);

        // Remove current window
        checkingWindows.removeOne(window);

        QList<TEdge*> movingEdges;
        TWindowList movingWindows;

        movingWindows.append(window);

        // Fill all moving edges into list
        QRect oldGeometry = window->geometry();
        int oldLeft = oldGeometry.left();
        int oldRight = oldGeometry.right();
        int oldTop = oldGeometry.top();
        int oldBottom = oldGeometry.bottom();

        int newLeft = newGeometry.left();
        int newRight = newGeometry.right();
        int newTop = newGeometry.top();
        int newBottom = newGeometry.bottom();

        if(oldLeft != newLeft)
            movingEdges.append(new TEdge(newTop, newBottom, newLeft, ET_LEFT));
        if(oldTop != newTop)
            movingEdges.append(new TEdge(newLeft, newRight, newTop, ET_TOP));
        if(oldRight != newRight)
            movingEdges.append(new TEdge(newTop, newBottom, newRight, ET_RIGHT));
        if(oldBottom != newBottom)
            movingEdges.append(new TEdge(newLeft, newRight, newBottom, ET_BOTTOM));

        // Fill all checking edges into list
        QList<TEdge*> checkingEdges;
        checkingWindows.removeOne(window);
        for(auto subWnd : checkingWindows)
        {
            if(subWnd->isVisible())
            {
                checkingEdges += subWnd->edges();
            }
        }

        // Add desktop edges
        checkingEdges += mDesktopWindow->edges();

        int minDLeft = INVALID_DISTANCE;
        int minDRight = INVALID_DISTANCE;
        int minDTop = INVALID_DISTANCE;
        int minDBottom = INVALID_DISTANCE;
        for(auto e : checkingEdges)
        {
            for(auto wndEdge : movingEdges)
            {
                int dis = wndEdge->parallelDistanceTo(e);
                if(dis!=0 && qAbs(dis)<=EDGE_CHECK_WIDTH)
                {
                    if(wndEdge->edgeType==ET_LEFT)
                        minDLeft = qMin(minDLeft, dis);
                    else if(wndEdge->edgeType==ET_RIGHT)
                        minDRight = qMin(minDRight, dis);
                    else if(wndEdge->edgeType==ET_TOP)
                        minDTop = qMin(minDTop, dis);
                    else if(wndEdge->edgeType==ET_BOTTOM)
                        minDBottom = qMin(minDBottom, dis);
                }
            }
        }
        if(minDLeft != INVALID_DISTANCE)
            newGeometry.setLeft(newLeft-minDLeft);
        if(minDRight != INVALID_DISTANCE)
            newGeometry.setRight(newRight-minDRight);
        if(minDTop != INVALID_DISTANCE)
            newGeometry.setTop(newTop-minDTop);
        if(minDBottom != INVALID_DISTANCE)
            newGeometry.setBottom(newBottom-minDBottom);

        // Destroy moving edges
        for(auto e : movingEdges)
            delete e;

        window->setGeometry(newGeometry);
    }
}

void TGuiManager::slotRequireShowLyricWindow()
{
    mLyricWindow->show();
    mShowDesktopLyric = false;
}

void TGuiManager::slotRequireShowDesktopLyric()
{
    mDesktopLyricWindow->show();
    mShowDesktopLyric = true;
}

void TGuiManager::slotShowMinimized()
{
    if(TPreferences::instance()->displayTrayIcon())
        hide();
    else
        mMainWindow->showMinimized();
}

void TGuiManager::slotRequestExit()
{
    TPreferences *prefs = TPreferences::instance();
    prefs->setWindowGeometryState(WT_MAIN, mMainWindow->saveGeometry(), mMainWindow->saveState());
    prefs->setWindowGeometryState(WT_LYRIC, mLyricWindow->saveGeometry(), mLyricWindow->saveState());
    prefs->setWindowGeometryState(WT_EQUALIZER, mEqualizerWindow->saveGeometry(), mEqualizerWindow->saveState());
    prefs->setWindowGeometryState(WT_PLAYLIST, mPlaylistWindow->saveGeometry(), mPlaylistWindow->saveState());

    emit requestShutdown();

    mMainWindow->close();
    mMiniWindow->close();
    mLyricWindow->close();
    mEqualizerWindow->close();
    mPlaylistWindow->close();
    mBrowserWindow->close();
    mDesktopLyricWindow->close();
    mDesktopWindow->close();
    mMainMenu->close();
}

void TGuiManager::slotOpenOptionsDialog()
{
    if(mOptionDialog)
        mOptionDialog->openPage(PI_GENERAL);
}

void TGuiManager::slotAbout()
{
    if(mOptionDialog)
        mOptionDialog->openPage(PI_ABOUT);
}

void TGuiManager::slotAboutQt()
{
    QMessageBox::aboutQt(mMainWindow);
}

void TGuiManager::slotVolumeToggled(bool checked)
{
    mMainMenu->volumeControlMenu()->checkMuteAction(checked);
    mMainWindow->checkMuteButton(checked);
    TPreferences::instance()->setMuteEnabled(checked);
    emit muteToggled(checked);
}

void TGuiManager::toggleGui()
{
    if(mMainWindow->isVisible())
    {
        if(!isMainwindowSunken())
            hide();
        else
            mMainWindow->activateWindow();
    } else {
        show();
    }
}

void TGuiManager::restoreGui()
{
    if(!mMainWindow->isVisible())
        open();
    else
        mMainWindow->activateWindow();
}

void TGuiManager::centerWindow()
{
    TWindowList movingWindows;
    movingWindows.append(mMainWindow);
    movingWindows.append(mLyricWindow);
    movingWindows.append(mEqualizerWindow);
    movingWindows.append(mPlaylistWindow);
    //movingWindows.append(mBrowserWindow);
    //movingWindows.append(mDesktopLyricWindow);

    QList<TEdge*> movingEdges;

    // Fill all moving edges into list
    for(auto wnd : movingWindows)
    {
        QRect movingRect = wnd->geometry();
        int newLeft = movingRect.left();
        int newRight = movingRect.right();
        int newTop = movingRect.top();
        int newBottom = movingRect.bottom();

        movingEdges.append(new TEdge(newTop, newBottom, newLeft, ET_LEFT));
        movingEdges.append(new TEdge(newLeft, newRight, newTop, ET_TOP));
        movingEdges.append(new TEdge(newTop, newBottom, newRight, ET_RIGHT));
        movingEdges.append(new TEdge(newLeft, newRight, newBottom, ET_BOTTOM));
    }

    // Find maximum boundary
    int desktopWidth = mDesktopWindow->width();
    int desktopHeight = mDesktopWindow->height();
    int left = desktopWidth;
    int top = desktopHeight;
    int right = 0;
    int bottom = 0;
    for(TEdge *e : movingEdges)
    {
        if(e->direction == ED_HORIZONTAL)
        {
            if(e->value < top)
                top = e->value;
            if(e->value > bottom)
                bottom = e->value;

        } else if(e->direction == ED_VERTICAL) {
            if(e->value < left)
                left = e->value;
            if(e->value > right)
                right = e->value;
        }
    }
    // Destroy moving edges
    for(auto e : movingEdges)
        delete e;

    int dx = (float)(desktopWidth - right + left) / 2 + 0.5;
    int dy = (float)(desktopHeight - bottom + top) / 2 + 0.5;;
    for(auto subWnd : movingWindows)
    {
        QPoint subWndPoint = subWnd->pos();
        subWndPoint.rx() += dx;
        subWndPoint.ry() += dy;
        subWnd->move(subWndPoint);
    }
}

void TGuiManager::slotOnOpacityChanged(qreal value)
{
    mMainWindow->setWindowOpacity(value);
    mEqualizerWindow->setWindowOpacity(value);
    mDesktopLyricWindow->setWindowOpacity(value);
    mLyricWindow->setWindowOpacity(value);
    mPlaylistWindow->setWindowOpacity(value);
    mBrowserWindow->setWindowOpacity(value);
}

void TGuiManager::slotRequestLoadSkin(int skinIndex)
{
    if(!mCurrentSkinFile.isEmpty())
    {
        TSkinSetting skinSetting(mCurrentSkinFile);
        skinSetting.writeMainWindowState(mMainWindow->saveGeometry(), mMainWindow->saveState());
        skinSetting.writeEqualizerWindowState(mEqualizerWindow->saveGeometry(), mEqualizerWindow->saveState());
        skinSetting.writeDesktopLyricWindowState(mDesktopLyricWindow->saveGeometry(), mDesktopLyricWindow->saveState());
        skinSetting.writeLyricWindowState(mLyricWindow->saveGeometry(), mLyricWindow->saveState());
        skinSetting.writePlaylistWindowState(mPlaylistWindow->saveGeometry(), mPlaylistWindow->saveState());
        skinSetting.writeBrowserWindowState(mBrowserWindow->saveGeometry(), mBrowserWindow->saveState());
    }
    TSkin *skin = mSkinManager->skinAt(skinIndex);
    if(skin)
        loadSkin(skin);
}

void TGuiManager::slotRequestSkinNames(QStringList &names)
{
    mSkinManager->reload();
    names = mSkinManager->skinNames();
}

void TGuiManager::slotMainWindowActivationChanged()
{
    if(mMainWindow->isActiveWindow())
    {
        mEqualizerWindow->raise();
        mLyricWindow->raise();
        mPlaylistWindow->raise();
    }
}

void TGuiManager::slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason==QSystemTrayIcon::Trigger)
    {
        toggleGui();
    } else if(reason==QSystemTrayIcon::DoubleClick) {
        restoreGui();
    }
}

void TGuiManager::slotVolumeUpTriggered()
{
    if(mMainWindow)
        mMainWindow->upVolume(0.1);
}

void TGuiManager::slotVolumeDownTriggered()
{
    if(mMainWindow)
        mMainWindow->upVolume(-0.1);
}

void TGuiManager::slotDisplayTrayIconToggled(bool checked)
{
    mTrayIcon->setVisible(checked);
}

void TGuiManager::hide()
{
    mBrowserVisible = mBrowserWindow->isVisible();
    mPlaylistVisible = mPlaylistWindow->isVisible();
    mLyricWindowVisible = mLyricWindow->isVisible();
    mDesktopLyricVisible = mDesktopLyricWindow->isVisible();
    mEqualizerVisible = mEqualizerWindow->isVisible();

    if(mEqualizerVisible)
        mEqualizerWindow->hide();

    if(mDesktopLyricVisible)
        mDesktopLyricWindow->hide();

    if(mLyricWindowVisible)
        mLyricWindow->hide();

    if(mPlaylistVisible)
        mPlaylistWindow->hide();

    if(mBrowserVisible)
        mBrowserWindow->hide();

    mMainWindow->hide();
}

void TGuiManager::show()
{
    if(mEqualizerVisible)
        mEqualizerWindow->showNormal();

    //if(mDesktopLyricVisible)
    //    mDesktopLyricWindow->showNormal();

    if(mLyricWindowVisible)
        mLyricWindow->showNormal();

    if(mPlaylistVisible)
        mPlaylistWindow->showNormal();

    //if(mBrowserVisible)
    //    mBrowserWindow->showNormal();

    mMainWindow->showNormal();
    mMainWindow->activateWindow();
}

void TGuiManager::exit()
{
    slotRequestExit();
}

bool TGuiManager::isMainwindowSunken()
{
    QRect rt = mMainWindow->geometry();
    QList<QPoint> checkPoint;
    checkPoint.append(QPoint(rt.left()+1, rt.top()+1));
    checkPoint.append(QPoint(rt.left()+1, rt.center().y()));
    checkPoint.append(QPoint(rt.center().x(), rt.top()+1));
    checkPoint.append(QPoint(rt.right()-1, rt.center().y()));
    checkPoint.append(QPoint(rt.center().x(), rt.bottom()-1));
    TMainWindow *mainWindow = NULL;
    for(QPoint pt : checkPoint)
    {
        mainWindow = qobject_cast<TMainWindow*>(qApp->widgetAt(pt.x(), pt.y()));
        if(mainWindow)
            break;
    }
    return mainWindow==NULL;
}

void TGuiManager::moveWindow(TAbstractWindow *window, int left, int top)
{
    TDesktopLyricWindow *desktopLyricWnd = qobject_cast<TDesktopLyricWindow *>(window);
    if(desktopLyricWnd)
    {
        // Desktop lyric window, move directly
        desktopLyricWnd->move(left, top);
        return;
    }

    QRect windowRect = window->geometry();
    int dx = left - windowRect.left();
    int dy = top - windowRect.top();

    TWindowList checkingWindows;
    //if(mMainWindow->isVisible())
        checkingWindows.append(mMainWindow);
    //if(mLyricWindow->isVisible())
        checkingWindows.append(mLyricWindow);
    //if(mEqualizerWindow->isVisible())
        checkingWindows.append(mEqualizerWindow);
    //if(mPlaylistWindow->isVisible())
        checkingWindows.append(mPlaylistWindow);
    //if(mBrowserWindow->isVisible())
        checkingWindows.append(mBrowserWindow);
    //if(mDesktopLyricWindow->isVisible())
        checkingWindows.append(mDesktopLyricWindow);

    if(window->mouseMoveTriggered())
    {
        TWindowList tempList = checkingWindows;
        // Store gumed sub windows
        mGumedWindows.clear();
        mGumedWindows.append(mMainWindow);
        findGumedWindows(tempList, mGumedWindows);
        // Remove main window
        mGumedWindows.removeAt(0);
    }

    QList<TEdge*> movingEdges;
    TWindowList movingWindows;

    movingWindows.append(window);

    TMainWindow *mainWindow = qobject_cast<TMainWindow *>(window);
    if(mainWindow)
    {
        movingWindows += mGumedWindows;
        // Check which window is gumed to main window, and remove them in checkingWindows
        for(auto subGumWnd : movingWindows)
        {
            checkingWindows.removeOne(subGumWnd);
        }
    }

    // Fill all moving edges into list
    for(auto wnd : movingWindows)
    {
        QRect movingRect = wnd->geometry();
        movingRect.moveTo(movingRect.left()+dx, movingRect.top()+dy);

        int newLeft = movingRect.left();
        int newRight = movingRect.right();
        int newTop = movingRect.top();
        int newBottom = movingRect.bottom();

        movingEdges.append(new TEdge(newTop, newBottom, newLeft, ET_LEFT));
        movingEdges.append(new TEdge(newLeft, newRight, newTop, ET_TOP));
        movingEdges.append(new TEdge(newTop, newBottom, newRight, ET_RIGHT));
        movingEdges.append(new TEdge(newLeft, newRight, newBottom, ET_BOTTOM));
    }

    // Fill all checking edges into list
    QList<TEdge*> checkingEdges;
    checkingWindows.removeOne(window);
    for(auto subWnd : checkingWindows)
    {
        if(subWnd->isVisible())
        {
            checkingEdges += subWnd->edges();
        }
    }

    // Add desktop edges
    checkingEdges += mDesktopWindow->edges();

    int minDx = INVALID_DISTANCE;
    int minDy = INVALID_DISTANCE;
    for(auto e : checkingEdges)
    {
        for(auto wndEdge : movingEdges)
        {
            int dis = wndEdge->distanceTo(e);
            if(dis!=0 && qAbs(dis)<=EDGE_CHECK_WIDTH)
            {
                if(wndEdge->direction==ED_HORIZONTAL)
                    minDy = qMin(minDy, dis);
                else
                    minDx = qMin(minDx, dis);
            }
        }
    }
    if(minDx != INVALID_DISTANCE)
        dx -= minDx;
    if(minDy != INVALID_DISTANCE)
        dy -= minDy;

    // Destroy moving edges
    for(auto e : movingEdges)
        delete e;

    for(auto subWnd : movingWindows)
    {
        QPoint subWndPoint = subWnd->pos();
        subWndPoint.rx() += dx;
        subWndPoint.ry() += dy;
        subWnd->move(subWndPoint);
    }
}

bool TGuiManager::loadSkin(TSkin *skin)
{
    if(!skin)
        return false;

    QDomElement root = skin->rootElement();
    mMainWindow->loadFromSkin(root.firstChildElement(TAG_PLAYER_WINDOW), skin);
    mMiniWindow->loadFromSkin(root.firstChildElement(TAG_MINI_WINDOW), skin);
    mLyricWindow->loadFromSkin(root.firstChildElement(TAG_LYRIC_WINDOW), skin);
    mEqualizerWindow->loadFromSkin(root.firstChildElement(TAG_EQUALIZER_WINDOW), skin);
    mPlaylistWindow->loadFromSkin(root.firstChildElement(TAG_PLAYLIST_WINDOW), skin);
    mBrowserWindow->loadFromSkin(root.firstChildElement(TAG_BROWSER_WINDOW), skin);
    mDesktopLyricWindow->loadFromSkin(root.firstChildElement(TAG_DESKLRC_BAR), skin);

    mTrayIcon->setIcon(mMainWindow->windowIcon());

    mCurrentSkinFile = skin->fileName();
    TPreferences::instance()->setSkinPath(mCurrentSkinFile);

    emit skinChanged();

    // Load skin setting
    TSkinSetting skinSetting(skin->fileName());
    if(skinSetting.exists())
    {
        QByteArray mainWindowGeometry;
        QByteArray mainWindowState;
        QByteArray equalizerWindowGeometry;
        QByteArray equalizerWindowState;
        QByteArray desktoplyricWindowGeometry;
        QByteArray desktoplyricWindowState;
        QByteArray lyricWindowGeometry;
        QByteArray lyricWindowState;
        QByteArray playlistWindowGeometry;
        QByteArray playlistWindowState;
        QByteArray browserWindowGeometry;
        QByteArray browserWindowState;

        skinSetting.mainWindowState(&mainWindowGeometry, &mainWindowState);
        skinSetting.equalizerWindowState(&equalizerWindowGeometry, &equalizerWindowState);
        skinSetting.desktopLyricWindowState(&desktoplyricWindowGeometry, &desktoplyricWindowState);
        skinSetting.lyricWindowState(&lyricWindowGeometry, &lyricWindowState);
        skinSetting.playlistWindowState(&playlistWindowGeometry, &playlistWindowState);
        skinSetting.browserWindowState(&browserWindowGeometry, &browserWindowState);

        if(!mainWindowGeometry.isEmpty())
            mMainWindow->restoreGeometry(mainWindowGeometry);
        if(!mainWindowState.isEmpty())
            mMainWindow->restoreState(mainWindowState);
        if(!equalizerWindowGeometry.isEmpty())
            mEqualizerWindow->restoreGeometry(equalizerWindowGeometry);
        if(!equalizerWindowState.isEmpty())
            mEqualizerWindow->restoreState(equalizerWindowState);
        if(!desktoplyricWindowGeometry.isEmpty())
            mDesktopLyricWindow->restoreGeometry(desktoplyricWindowGeometry);
        if(!desktoplyricWindowState.isEmpty())
            mDesktopLyricWindow->restoreState(desktoplyricWindowState);
        if(!lyricWindowGeometry.isEmpty())
            mLyricWindow->restoreGeometry(lyricWindowGeometry);
        if(!lyricWindowState.isEmpty())
            mLyricWindow->restoreState(lyricWindowState);
        if(!playlistWindowGeometry.isEmpty())
            mPlaylistWindow->restoreGeometry(playlistWindowGeometry);
        if(!playlistWindowState.isEmpty())
            mPlaylistWindow->restoreState(playlistWindowState);
        if(!browserWindowGeometry.isEmpty())
            mBrowserWindow->restoreGeometry(browserWindowGeometry);
        if(!browserWindowState.isEmpty())
            mBrowserWindow->restoreState(browserWindowState);
    } else {
        // First created
        centerWindow();
    }

    return true;
}
