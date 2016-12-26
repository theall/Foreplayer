#include "guimanager.h"

#include <QMessageBox>

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
    , mMainMenu(new TMainMenu)
    , mMinimode(false)
    , mShowDesktopLyric(false)
{
    mMainWindow->setContextMenu(mMainMenu);

    // Main window
    connect(mMainWindow, SIGNAL(exitClicked()), this, SLOT(slotRequestExit()));
    connect(mMainWindow, SIGNAL(lyricButtonToggle(bool)), this, SLOT(slotLyricButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(equalizerButtonToggle(bool)), this, SLOT(slotEqualizerButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(playlistButtonToggle(bool)), this, SLOT(slotPlaylistButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(browserButtonToggle(bool)), this, SLOT(slotBrowserButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));
    connect(mMainWindow, SIGNAL(requestShowMinimized()), this, SLOT(slotShowMinimized()));
    connect(mMainWindow, SIGNAL(requestToggleWindow()), this, SLOT(slotRequestToggleWindow()));
    connect(mMainWindow, SIGNAL(requestRestoreWindow()), this, SLOT(slotRequestRestoreWindow()));
    connect(mMainWindow, SIGNAL(onActivationChange()), this, SLOT(slotMainWindowActivationChanged()));
    connect(mMainWindow, SIGNAL(requestClose()), this, SLOT(slotMainWindowAboutToClose()));

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

    //Main menu
    mMainMenu->skinMenu()->setPath("z:/skins");
    connect(mMainMenu, SIGNAL(onExitTriggered()), this, SLOT(slotExit()));
    connect(mMainMenu->skinMenu(), SIGNAL(requestLoadSkin(QString)), this, SLOT(slotRequestLoadSkin(QString)));

    connect(mMainMenu->transparentMenu(), SIGNAL(onOpacityChanged(qreal)), this, SLOT(slotOnOpacityChanged(qreal)));
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
    if(mMainMenu)
    {
        delete mMainMenu;
        mMainMenu = NULL;
    }
}

void TGuiManager::loadSkin(QString fileName)
{
    if(fileName.isEmpty())
        return;

    TSkin skin;
    if(skin.load(fileName))
    {
        QDomElement root = skin.rootElement();
        mMainWindow->loadFromSkin(root.firstChildElement(TAG_PLAYER_WINDOW), &skin);
        mMiniWindow->loadFromSkin(root.firstChildElement(TAG_MINI_WINDOW), &skin);
        mLyricWindow->loadFromSkin(root.firstChildElement(TAG_LYRIC_WINDOW), &skin);
        mEqualizerWindow->loadFromSkin(root.firstChildElement(TAG_EQUALIZER_WINDOW), &skin);
        mPlaylistWindow->loadFromSkin(root.firstChildElement(TAG_PLAYLIST_WINDOW), &skin);
        mBrowserWindow->loadFromSkin(root.firstChildElement(TAG_BROWSER_WINDOW), &skin);
        mDesktopLyricWindow->loadFromSkin(root.firstChildElement(TAG_DESKLRC_BAR), &skin);
    } else {
        QMessageBox::critical(
            mMainWindow,
            tr("Load skin error."),
            skin.lastError());
    }
}

void TGuiManager::showGui()
{
    mMainWindow->checkLyricButton(true);
    mMainWindow->checkEqualizerButton(true);
    mMainWindow->checkPlaylistButton(true);

    mMainWindow->show();
    mLyricWindow->show();
    mEqualizerWindow->show();
    mPlaylistWindow->show();
    //mBrowserWindow->show();
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
    hide();
}

void TGuiManager::slotRequestExit()
{
    mMainWindow->close();
}

void TGuiManager::slotRequestToggleWindow()
{
    if(mMainWindow->isVisible())
    {
        hide();
    } else {
        show();
    }
}

void TGuiManager::slotRequestRestoreWindow()
{
    if(!mMainWindow->isVisible())
        show();
    else
        mMainWindow->activateWindow();
}

void TGuiManager::slotExit()
{
    exit(0);
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

void TGuiManager::slotRequestLoadSkin(QString skinFullName)
{
    loadSkin(skinFullName);
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

void TGuiManager::slotMainWindowAboutToClose()
{
    mLyricWindow->close();
    mEqualizerWindow->close();
    mPlaylistWindow->close();
    mBrowserWindow->close();
    mDesktopLyricWindow->close();
    mDesktopWindow->close();
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
