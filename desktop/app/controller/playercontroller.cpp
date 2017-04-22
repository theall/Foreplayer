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

TPlayerController::TPlayerController(QObject *parent) :
    TAbstractController(parent)
  , mCurrentItem(NULL)
{

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

    return TAbstractController::joint(gui, core);;
}

void TPlayerController::slotRequestPlay(int pIndex, int mIndex, int tIndex)
{
    if(!mCore || !mCore || !mMainWindow)
        return;

    slotStopButtonClicked();

    mCurrentItem = NULL;

    int newPL = -1;
    int newML = -1;
    int newTL = -1;

    PlayListItem playlistItem = mCore->getPlaylistItem(pIndex);
    if(playlistItem)
    {
        MusicItem musicItem = mCore->getMusicItem(playlistItem, mIndex);
        if(musicItem)
        {
            TrackItem trackItem = mCore->getTrackItem(musicItem, tIndex);
            bool playing = mCore->playTrackItem(trackItem);
            if(playing)
            {
                newPL = pIndex;
                newML = mIndex;
                newTL = tIndex;
                mCurrentItem = trackItem;
            } else {
                mCore->getPlayingIndex(&newPL, &newML, &newTL);
            }
        }
    }

    // Set playing index to playlist core
    mCore->setPlayingIndex(newPL, newML, newTL);

    // Set playing index to models
    emit requestUpdateModelsPlayingIndex(newPL, newML, newTL);

    updateWindowTitles();
}

void TPlayerController::slotPlayButtonClicked()
{
    if(!mCore)
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
    mMainWindow->setButtonPlayChecked(true);
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
            ti = 0;
            mi--;
            if(mi < 0)
            {
                mi = mCore->getMusicItemCount(playlistItem) - 1;
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
    mMainWindow->setButtonPlayChecked(true);
    mMainWindow->setPlayState(tr("Stoped"));
    mMainWindow->setProgress(0, 0);
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
        mMainWindow->setButtonPlayChecked(false);
        startTimer(40);
    } else {
        stopTimer();
        mGui->setCaption("");
        mMainWindow->setTitles(QStringList()<<tr("Play failed."));
        mMainWindow->setPlayState(tr("Stoped"));
        mMainWindow->setButtonPlayChecked(true);
        resetVisualWidget();
    }
}

void TPlayerController::delayStopTimer()
{
    if(!mCore || mCore->isPaused() || mCore->isStoped())
        stopTimer();
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
            int playedTime = mCore->getCurrentPlayedTime();
            int fakeDuration = mCore->getTrackItemDuration(mCurrentItem);
            bool needCheck = false;
            if(fakeDuration <= 0)
            {
                needCheck = true;
                fakeDuration = 150000;
            }
            mMainWindow->setProgress(playedTime, fakeDuration);
            if(needCheck)
            {
                int silentMSecs = 0;
                mCore->getAudioData(ADT_SILENT_MICRO_SECONDS, &silentMSecs, NULL);
                if(silentMSecs > 3000)
                {
                    // Fix up duration manually
                    if(TPreferences::instance()->autoCorrectDuration())
                        emit requestFixDuration(playedTime-3000);
                    slotNextButtonClicked();                    
                }
            }
            if(fakeDuration+500 <= playedTime) {
                slotNextButtonClicked();
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
