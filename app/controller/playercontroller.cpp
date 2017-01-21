#include "playercontroller.h"

TPlayerController::TPlayerController(QObject *parent) :
    TAbstractController(parent)
{

}

void TPlayerController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    TAbstractController::joint(gui, core);

    mPlayerCore = core->player();
    Q_ASSERT(mPlayerCore);

    mPlaylistCore = core->playlist();
    Q_ASSERT(mPlaylistCore);

    mMainWindow = gui->mainWindow();
    Q_ASSERT(mMainWindow);

    connect(mMainWindow, SIGNAL(playClicked()), this, SLOT(slotPlayButtonClicked()));
    connect(mMainWindow, SIGNAL(pauseClicked()), this, SLOT(slotPauseButtonClicked()));
    connect(mMainWindow, SIGNAL(prevClicked()), this, SLOT(slotPrevButtonClicked()));
    connect(mMainWindow, SIGNAL(nextClicked()), this, SLOT(slotNextButtonClicked()));
    connect(mMainWindow, SIGNAL(stopClicked()), this, SLOT(slotStopButtonClicked()));

    QStringList titles;
    titles.append("13: 岁月无声 beyond IV (from 1983-2003) contributed");
    titles.append("标题: 岁月无声 beyond IV");
    titles.append("艺术家: beyond beyond IV");
    titles.append("专辑: beyond IV (from 1983-2003) contributed to beyond.Spirit of wong.");
    titles.append("格式: MP3 44KHZ 320K");

    mMainWindow->setCaption("fulkfour is coming!");
    mMainWindow->setTitles(titles);
    mMainWindow->setTime(99, 354);
}

void TPlayerController::slotRequestPlay(int pIndex, int mIndex, int tIndex)
{
    if(!mPlaylistCore || !mPlayerCore)
        return;

    TPlaylistItem *playlistItem = mPlaylistCore->playlistItem(pIndex);
    if(playlistItem)
    {
        TMusicItem *musicItem = playlistItem->musicItem(mIndex);
        if(musicItem)
        {
            TTrackItem *trackItem = musicItem->trackItem(tIndex);
            bool playing = trackItem && mPlayerCore->playTrack(trackItem);
            if(playing)
            {
                // Set playing index to playlist core
                mPlaylistCore->setPlayingIndex(pIndex, mIndex, tIndex);

                // Set playing index to models
                emit requestUpdateModelsPlayingIndex(pIndex, mIndex, tIndex);
            }
            mMainWindow->setButtonPlayChecked(!playing);
        }
    } else {
        emit requestUpdateModelsPlayingIndex(-1, -1, -1);
    }
}

void TPlayerController::slotPlayButtonClicked()
{
    if(!mPlaylistCore)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mPlaylistCore->playingIndex(&pi, &mi, &ti);
    slotRequestPlay(pi, mi, ti);
}

void TPlayerController::slotPauseButtonClicked()
{
    if(!mPlayerCore || !mMainWindow)
        return;

    mPlayerCore->pause();
    mMainWindow->setButtonPlayChecked(true);
}

void TPlayerController::slotPrevButtonClicked()
{
    if(!mPlayerCore || !mMainWindow)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mPlaylistCore->playingIndex(&pi, &mi, &ti);
    if(TPlaylistItem *playlistItem=mPlaylistCore->playlistItem(pi))
    {
        ti--;

        // Default recycle level is in track list
        if(ti < 0)
        {
            ti = 0;
            mi--;
            if(mi < 0)
            {
                mi = playlistItem->size() - 1;
            }
        }
        slotRequestPlay(pi, mi, ti);
    }
}

void TPlayerController::slotNextButtonClicked()
{
    if(!mPlayerCore || !mMainWindow)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mPlaylistCore->playingIndex(&pi, &mi, &ti);
    if(TPlaylistItem *playlistItem=mPlaylistCore->playlistItem(pi))
    {
        TMusicItem *musicItem = playlistItem->musicItem(mi);
        ti++;

        // Default recycle level is in track list
        if(ti >= musicItem->size())
        {
            mi++;
            if(mi >= playlistItem->size())
            {
                mi = 0;
            }
            ti = 0;
        }
        slotRequestPlay(pi, mi, ti);
    }
}

void TPlayerController::slotStopButtonClicked()
{
    if(!mPlayerCore || !mMainWindow)
        return;
}

void TPlayerController::slotTimerEvent()
{
}
