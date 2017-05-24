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
#include "playercontroller.h"
#include "preferences.h"

#include <QDateTime>

TPlayerController::TPlayerController(QObject *parent) :
    TAbstractController(parent)
  , mCurrentItem(NULL)
{
    srand(QDateTime::currentMSecsSinceEpoch());
}

TPlayerController::~TPlayerController()
{
}

bool TPlayerController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    mMainWindow = gui->mainWindow();
    Q_ASSERT(mMainWindow);

    connect(mMainWindow, SIGNAL(playClicked()), this, SLOT(slotPlayButtonClicked()));
    connect(mMainWindow, SIGNAL(pauseClicked()), this, SLOT(slotPauseButtonClicked()));
    connect(mMainWindow, SIGNAL(prevClicked()), this, SLOT(slotPrevButtonClicked()));
    connect(mMainWindow, SIGNAL(nextClicked()), this, SLOT(slotNextButtonClicked()));
    connect(mMainWindow, SIGNAL(stopClicked()), this, SLOT(slotStopButtonClicked()));
    connect(mMainWindow, SIGNAL(requestSeekPosition(int,bool&)), this, SLOT(slotRequestSeekPosition(int,bool&)));
    connect(mMainWindow, SIGNAL(volumeValueChanged(float)), this, SLOT(slotVolumeValueChanged(float)));
    connect(gui, SIGNAL(muteToggled(bool)), this, SLOT(slotVolumeToggled(bool)));

    TMainMenu *mainMenu = gui->mainMenu();
    Q_ASSERT(mainMenu);

    connect(mainMenu->playControlMenu(), SIGNAL(onPlayTriggered()), this, SLOT(slotPlayButtonClicked()));
    connect(mainMenu->playControlMenu(), SIGNAL(onPauseTriggered()), this, SLOT(slotPauseButtonClicked()));
    connect(mainMenu->playControlMenu(), SIGNAL(onStopTriggered()), this, SLOT(slotStopButtonClicked()));
    connect(mainMenu->playControlMenu(), SIGNAL(onNextTrackTriggered()), this, SLOT(slotNextButtonClicked()));
    connect(mainMenu->playControlMenu(), SIGNAL(onPrevTrackTriggered()), this, SLOT(slotPrevButtonClicked()));
    connect(mainMenu->playControlMenu(), SIGNAL(onPrevMusicTriggered()), this, SLOT(slotPrevMusicTriggered()));
    connect(mainMenu->playControlMenu(), SIGNAL(onNextMusicTriggered()), this, SLOT(slotNextMusicTriggered()));

    return TAbstractController::joint(gui, core);
}

void TPlayerController::slotRequestPlay(int pIndex, int mIndex, int tIndex)
{
    if(!mCore || !mCore || !mMainWindow)
        return;

    slotStopButtonClicked();

    mCurrentItem = NULL;

    PlayListItem playlistItem = mCore->getPlaylistItem(pIndex);
    if(playlistItem)
    {
        MusicItem musicItem = mCore->getMusicItem(playlistItem, mIndex);
        if(musicItem)
        {
            TrackItem trackItem = mCore->getTrackItem(musicItem, tIndex);
            bool playing = mCore->playTrackItem(trackItem);
            if(playing)
                mCurrentItem = trackItem;
            else
                mCurrentItem = NULL;
        }
    }

    // Set playing index to playlist core
    mCore->setPlayingIndex(pIndex, mIndex, tIndex);

    // Set playing index to models
    emit requestUpdateModelsPlayingIndex(pIndex, mIndex, tIndex);

    updateWindowTitles();
}

void TPlayerController::slotPlayButtonClicked()
{
    if(!mCore || mCore->isPlaying())
        return;

    // Try resume
    int pi = -1;
    int mi = -1;
    int ti = -1;
    if(mCore->isPaused())
    {
        if(mCore->resume()) {
            mCurrentItem = mCore->getPlayingTrackItem();
            updateWindowTitles();
        } else {
            mCore->getPlayingIndex(&pi, &mi, &ti);
            slotRequestPlay(pi, mi, ti);
        }
    } else {
        // Get current index from playlist controller
        emit requestCurrentIndex(&pi, &mi, &ti);
        slotRequestPlay(pi, mi, ti);
    }
}

void TPlayerController::slotPauseButtonClicked()
{
    if(!mCore || !mMainWindow)
        return;

    mCore->pause();
    QTimer::singleShot(3000, this, SLOT(delayStopTimer()));
    mMainWindow->setButtonPlayVisible(true);
    mMainWindow->setPlayState(tr("Paused"));
}

void TPlayerController::slotPrevButtonClicked()
{
    if(!mCore || !mMainWindow)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mCore->getPlayingIndex(&pi, &mi, &ti);
    if(PlayListItem playlistItem=mCore->getPlaylistItem(pi))
    {
        ti--;

        // Default recycle level is in track list
        if(ti < 0)
        {
            mi--;
            if(mi < 0)
            {
                mi = mCore->getMusicItemCount(playlistItem) - 1;
            } else {
                MusicItem musicItem = mCore->getMusicItem(playlistItem, mi);
                ti = mCore->getTrackItemCount(musicItem) - 1;
            }
        }
        slotRequestPlay(pi, mi, ti);
    }
}

void TPlayerController::slotNextButtonClicked()
{
    if(!mCore)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mCore->getPlayingIndex(&pi, &mi, &ti);
    if(PlayListItem playlistItem=mCore->getPlaylistItem(pi))
    {
        MusicItem musicItem = mCore->getMusicItem(playlistItem, mi);
        if(musicItem)
        {
            ti++;

            // Default recycle level is in track list
            if(ti >= mCore->getTrackItemCount(musicItem))
            {
                mi++;
                if(mi >= mCore->getMusicItemCount(playlistItem))
                {
                    mi = 0;
                }
                ti = 0;
            }
            slotRequestPlay(pi, mi, ti);
        }
    }
}

void TPlayerController::slotStopButtonClicked()
{
    if(!mCore || !mMainWindow)
        return;

    mCore->stop();
    QTimer::singleShot(3000, this, SLOT(delayStopTimer()));
    mMainWindow->setButtonPlayVisible(true);
    mMainWindow->setPlayState(tr("Stoped"));
    mMainWindow->setProgress(0, 0);
}

void TPlayerController::slotRequestSeekPosition(int newValue, bool &result)
{
    result = mCore && mCore->seek(newValue);
}

void TPlayerController::slotVolumeValueChanged(float value)
{
    if(mCore)
        mCore->setAudioParameter(AP_VOLUME, value);
}

void TPlayerController::slotVolumeToggled(bool toggled)
{
    if(mCore)
        mCore->setAudioParameter(AP_VOLUME_ENABLE, (float)!toggled);
}

void TPlayerController::slotPrevMusicTriggered()
{
    if(!mCore)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mCore->getPlayingIndex(&pi, &mi, &ti);
    if(PlayListItem playlistItem=mCore->getPlaylistItem(pi))
    {
        mi--;
        ti = 0;
        // Default recycle level is in track list
        if(mi < 0)
            mi = mCore->getMusicItemCount(playlistItem) - 1;

        slotRequestPlay(pi, mi, ti);
    }
}

void TPlayerController::slotNextMusicTriggered()
{
    if(!mCore)
        return;

    int pi = -1;
    int mi = -1;
    int ti = -1;
    mCore->getPlayingIndex(&pi, &mi, &ti);
    if(PlayListItem playlistItem=mCore->getPlaylistItem(pi))
    {
        mi++;
        ti = 0;
        // Default recycle level is in track list
        if(mi >= mCore->getMusicItemCount(playlistItem))
            mi = 0;

        slotRequestPlay(pi, mi, ti);
    }
}

void TPlayerController::updateWindowTitles()
{
    if(!mMainWindow || !mGui)
        return;

    if(mCurrentItem)
    {
        QStringList titles;
        titles.append(mCore->getTrackItemName(mCurrentItem));
        titles.append(mCore->getTrackItemAdditionalInfo(mCurrentItem).split("\n"));
        mGui->setCaption(mCore->getTrackItemName(mCurrentItem));
        mMainWindow->setTitles(titles);
        mMainWindow->setPlayState(tr("Playing"));
        mMainWindow->setButtonPlayVisible(false);
        startMyTimer(40);
    } else {
        stopMyTimer();
        mGui->setCaption("");
        mMainWindow->setTitles(QStringList()<<tr("Play failed."));
        mMainWindow->setPlayState(tr("Stoped"));
        mMainWindow->setButtonPlayVisible(true);
        resetVisualWidget();
    }
}

void TPlayerController::decidePlayNext()
{
    if(TPreferences::instance()->playMode()==PM_MANUAL)
    {
        slotStopButtonClicked();
    } else {
        int pi = -1;
        int mi = -1;
        int ti = -1;
        int errorCount = 0;
        while(true)
        {
            if(!getNextPlayindex(&pi, &mi, &ti))
            {
                slotStopButtonClicked();
                break;
            }

            slotRequestPlay(pi, mi, ti);
            if(!mCurrentItem)
            {
                errorCount++;
                if(errorCount > 3)
                {
                    slotStopButtonClicked();
                    break;
                }
            } else {
                errorCount = 0;
                break;
            }
        }
    }
}

bool TPlayerController::getNextPlayindex(int *pIndex, int *mIndex, int *tIndex)
{
    if(!mCore)
        return false;

    PlayMode pm = TPreferences::instance()->playMode();
    int pi = -1;
    int mi = -1;
    int ti = -1;
    QMap<int, QMap<int, int>> validIndexList;
    for(int i=0;i<mCore->playlistCount();i++)
    {
        PlayListItem pItem = mCore->getPlaylistItem(i);
        int musicItemsCount = mCore->getMusicItemCount(pItem);
        if(musicItemsCount > 0)
        {
            QMap<int, int> musicIndexMap;
            for(int j=0;j<mCore->getMusicItemCount(pItem);j++)
            {
                MusicItem mItem = mCore->getMusicItem(pItem, j);
                int trackItemsCount = mCore->getTrackItemCount(mItem);
                if(trackItemsCount > 0)
                {
                    musicIndexMap.insert(j, trackItemsCount);
                }
            }
            if(musicIndexMap.size() > 0)
                validIndexList.insert(i, musicIndexMap);
        }
    }
    if(validIndexList.size() < 1)
        return false;

    if(pm == PM_RANDOM)
    {
        QList<int> pIndexList = validIndexList.keys();
        int playlistCount = pIndexList.size();
        int pIndex = ((float)rand()/RAND_MAX) * playlistCount-1;
        if(pIndex >= playlistCount)
            pIndex = playlistCount - 1;
        if (pIndex < 0)
            pIndex = 0;

        pi = pIndexList[pIndex];
        QMap<int, int> musicCountMap = validIndexList[pi];
        QList<int> musicIndexList = musicCountMap.keys();
        int musicCount = musicIndexList.size();
        mi = ((float)rand()/RAND_MAX) * musicCount;
        if(mi >= musicCount)
            mi = musicCount - 1;
        if (mi < 0)
            mi = 0;

        int trackCount = musicCountMap[musicIndexList[mi]];
        ti = ((float)rand()/RAND_MAX) * trackCount;
        if(ti >= trackCount)
            ti = trackCount - 1;
        else if (ti < 0)
            ti = 0;
    } else {
        mCore->getPlayingIndex(&pi, &mi, &ti);
        QMap<int, QMap<int, int>>::iterator playlistMapIt = validIndexList.lowerBound(pi);
        if(playlistMapIt==validIndexList.end())
        {
            // pi==-1?, reset play order
            pi = validIndexList.firstKey();
            mi = 0;
            ti = 0;
        } else {
            if(pi != playlistMapIt.key())
            {
                // Current item is removed
                pi = playlistMapIt.key();
                mi = playlistMapIt.value().firstKey();
                ti = 0;
            } else {
                QMap<int, int> musicCountMap = playlistMapIt.value();
                QMap<int, int>::iterator musicCountMapIt = musicCountMap.lowerBound(mi);
                if(musicCountMapIt==musicCountMap.end() || musicCountMapIt.key()!=mi)
                {
                    // Invalid music index?
                    mi = musicCountMap.firstKey();
                    ti = 0;
                } else {
                    if(pm != PM_RECYCLE_TRACK)
                    {
                        int trackCount = musicCountMapIt.value();
                        ti++;

                        if(ti >= trackCount)
                        {
                            ti = 0;
                            if(pm==PM_RECYCLE_PLAY_LIST || pm==PM_RECYCLE_ALL)
                            {
                                musicCountMapIt++;
                                if(musicCountMapIt == musicCountMap.end())
                                {
                                    if(pm==PM_RECYCLE_ALL)
                                    {
                                        playlistMapIt++;
                                        if(playlistMapIt==validIndexList.end())
                                        {
                                            playlistMapIt = validIndexList.begin();
                                            musicCountMapIt = playlistMapIt.value().begin();
                                        }
                                        pi = playlistMapIt.key();
                                    } else if(pm==PM_RECYCLE_PLAY_LIST) {
                                        musicCountMapIt = musicCountMap.begin();
                                    }
                                }
                                mi = musicCountMapIt.key();
                            }
                        }
                    }
                }
            }
        }
    }

    qDebug() << "Next play index" << pi << mi << ti;
    *pIndex = pi;
    *mIndex = mi;
    *tIndex = ti;

    return true;
}

void TPlayerController::delayStopTimer()
{
    if(!mCore || mCore->isPaused() || mCore->isStoped())
        stopMyTimer();
}

void TPlayerController::resetVisualWidget()
{
    if(mMainWindow && mMainWindow->visualWidget())
        mMainWindow->visualWidget()->reset();
}

void TPlayerController::slotTimerEvent()
{
    if(!mCore || !mMainWindow)
        return;

    if(mCurrentItem)
    {
        if(mCore->isPlaying())
        {
            TPreferences *prefs = TPreferences::instance();
            int playedTime = mCore->getCurrentPlayedTime();
            int fakeDuration = mCore->getTrackItemDuration(mCurrentItem);
            bool needCheck = false;
            if(fakeDuration <= 0)
            {
                needCheck = true;
                fakeDuration = prefs->pilotDuration();
            }
            mMainWindow->setProgress(playedTime, fakeDuration);
            if(needCheck || TPreferences::instance()->forceCorrectDuration())
            {
                int silentMSecs = 0;
                mCore->getAudioData(ADT_SILENT_MICRO_SECONDS, &silentMSecs, NULL);
                int checkDuration = prefs->checkDuration();
                if(silentMSecs > checkDuration)
                {
                    // Fix up duration manually
                    if(TPreferences::instance()->autoCorrectDuration())
                        emit requestFixDuration(playedTime-checkDuration);
                    decidePlayNext();
                }
            }
            if(fakeDuration+100 <= playedTime) {
                decidePlayNext();
            }
        }

        // Update spectrum bar
        TVisualWidget *vw = mMainWindow->visualWidget();
        if(vw && !mCore->isStoped())
        {
            int size = 0;
            if(vw->spectrumMode())
            {
                TSpectrumElement *spectrumArray = NULL;
                mCore->getAudioData(ADT_SPECTRUM, (void*)&spectrumArray, (void*)&size);

                if(size>0 && spectrumArray) {
                    float levels[size];
                    for(int i = 0; i < size; i++)
                        levels[i] = spectrumArray[i].amplitude;
                    vw->setValue(levels, size);
                }
            } else {
                short *samples = NULL;
                mCore->getAudioData(ADT_SAMPLE, (void*)&samples, (void*)&size);
                if(size > 0)
                {
                    float levels[size];
                    for(int i = 0; i < size; i++)
                        levels[i] = 8*(float)samples[i] / 0x8000;
                    vw->setValue(levels, size);
                }
            }
        } else {
            resetVisualWidget();
        }

    } else {
        mMainWindow->setProgress(0, 0);
    }
}
