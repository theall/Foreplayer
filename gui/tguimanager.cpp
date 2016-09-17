#include "tguimanager.h"

#include "skinloader.h"

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

TGuiManager::TGuiManager(QObject *parent) :
    QObject(parent),
    mMainWindow(new TMainWindow),
    mLyricWindow(new TLyricWindow),
    mEqualizerWindow(new TEqualizerWindow),
    mPlaylistWindow(new TPlaylistWindow),
    mBrowserWindow(new TBrowserWindow),
    mDesktopLyricWindow(new TDesktopLyricWindow),
    mDesktopWindow(new TDesktopWindow),
    mMainMenu(new TMainMenu),
    mShowDesktopLyric(false)
{
    QStringList titles;
    titles.append("13: 岁月无声 beyond IV (from 1983-2003) contributed");
    titles.append("标题: 岁月无声 beyond IV");
    titles.append("艺术家: beyond beyond IV");
    titles.append("专辑: beyond IV (from 1983-2003) contributed to beyond.Spirit of wong.");
    titles.append("格式: MP3 44KHZ 320K");

    mMainWindow->setCaption("fulkfour is coming!");
    mMainWindow->setTitles(titles);
    mMainWindow->setTime(99, 354);
    mMainWindow->setContextMenu(mMainMenu);

    // Main window
    connect(mMainWindow, SIGNAL(lyricButtonToggle(bool)), this, SLOT(slotLyricButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(equalizerButtonToggle(bool)), this, SLOT(slotEqualizerButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(playlistButtonToggle(bool)), this, SLOT(slotPlaylistButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(browserButtonToggle(bool)), this, SLOT(slotBrowserButtonToggled(bool)));
    connect(mMainWindow, SIGNAL(requestMoveWindow(QPoint)), this, SLOT(slotRequestMoveWindow(QPoint)));
    connect(mMainWindow, SIGNAL(requestShowMinimized()), this, SLOT(slotShowMinimized()));
    connect(mMainWindow, SIGNAL(requestToggleWindow()), this, SLOT(slotRequestToggleWindow()));
    connect(mMainWindow, SIGNAL(requestRestoreWindow()), this, SLOT(slotRequestRestoreWindow()));

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
    connect(mMainMenu, SIGNAL(onExitTriggered()), this, SLOT(slotExit()));
}

void TGuiManager::loadSkin(QString skinPath)
{
    SkinLoader loader;
    loader.load(skinPath);

    mMainWindow->setGuiParameter(loader.mainWindow(), loader.miniWindow());

    QRect rt;

    rt = mMainWindow->geometry();

    //The window position is releative to main window
    LyricWindowParam *lyricParam = loader.lyricWindow();
    rt = lyricParam->window.position.geometry();
    mLyricWindow->setGuiParamter(lyricParam);
    mLyricWindow->move(rt.x(), rt.y());

    EqualizerWindowParam *equalizerParam = loader.equalizerWindow();
    rt = equalizerParam->window.position.geometry();
    mEqualizerWindow->setGuiParamter(equalizerParam);
    mEqualizerWindow->move(rt.x(), rt.y());

    PlaylistWindowParam *playlistParam = loader.playlistWindow();
    rt = playlistParam->window.position.geometry();
    mPlaylistWindow->setGuiParamter(playlistParam);
    mPlaylistWindow->move(rt.x(), rt.y());

    BrowserWindowParam *browserParam = loader.browserWindow();
    rt = browserParam->window.position.geometry();
    mBrowserWindow->setGuiParamter(browserParam);
    mBrowserWindow->move(rt.x(), rt.y());

    DesklrcBarParam *desktoplyricParam = loader.desklrcBar();
    rt = desktoplyricParam->window.position.geometry();
    mDesktopLyricWindow->setGuiParamter(desktoplyricParam);
    mDesktopLyricWindow->move(rt.x(), rt.y());
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

TLyricWindow *TGuiManager::lyricWindow()
{
    return mLyricWindow;
}

TMainWindow *TGuiManager::mainWindow()
{
    return mMainWindow;
}

TEqualizerWindow *TGuiManager::equalizerWindow()
{
    return mEqualizerWindow;
}

TBrowserWindow *TGuiManager::browserWindow()
{
    return mBrowserWindow;
}

TDesktopLyricWindow *TGuiManager::desktopLyricWindow()
{
    return mDesktopLyricWindow;
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
        int newLeft = newGeometry.left();
        int newRight = newGeometry.right();
        int newTop = newGeometry.top();
        int newBottom = newGeometry.bottom();

        movingEdges.append(new TEdge(newTop, newBottom, newLeft, ET_LEFT));
        movingEdges.append(new TEdge(newLeft, newRight, newTop, ET_TOP));
        movingEdges.append(new TEdge(newTop, newBottom, newRight, ET_RIGHT));
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
                int dis = wndEdge->distanceTo(e);
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

void TGuiManager::slotRequestToggleWindow()
{
    if(mMainWindow->isVisible())
        hide();
    else
        show();
}

void TGuiManager::slotRequestRestoreWindow()
{
    show();
}

void TGuiManager::slotExit()
{
    exit(0);
}

void TGuiManager::hide()
{
    mBrowserVisiable = mBrowserWindow->isVisible();
    mPlaylistVisiable = mPlaylistWindow->isVisible();
    mLyricWindowVisiable = mLyricWindow->isVisible();
    mDesktopLyricVisiable = mDesktopLyricWindow->isVisible();
    mEqualizerVisiable = mEqualizerWindow->isVisible();

    if(mEqualizerVisiable)
        mEqualizerWindow->hide();

    if(mDesktopLyricVisiable)
        mDesktopLyricWindow->hide();

    if(mLyricWindowVisiable)
        mLyricWindow->hide();

    if(mPlaylistVisiable)
        mPlaylistWindow->hide();

    if(mBrowserVisiable)
        mBrowserWindow->hide();

    mMainWindow->hide();
}

void TGuiManager::show()
{
    mMainWindow->show();

    if(mEqualizerVisiable)
        mEqualizerWindow->show();

    if(mDesktopLyricVisiable)
        mDesktopLyricWindow->show();

    if(mLyricWindowVisiable)
        mLyricWindow->show();

    if(mPlaylistVisiable)
        mPlaylistWindow->show();

    if(mBrowserVisiable)
        mBrowserWindow->show();

    QApplication::setActiveWindow(mMainWindow);
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
