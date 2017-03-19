#include "playercontroller.h"

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
    connect(mMainWindow, SIGNAL(volumeValueChanged(float)), this, SLOT(slotVolumeValueChanged(float)));
    connect(mMainWindow, SIGNAL(volumeToggle(bool)), this, SLOT(slotVolumeToggled(bool)));

    return TAbstractController::joint(gui, core);;
}

void TPlayerController::slotRequestPlay(int pIndex, int mIndex, int tIndex)
{
    if(!mPlaylistCore || !mPlayerCore || !mMainWindow)
        return;

    slotStopButtonClicked();

    mCurrentItem = NULL;

    int newPL = -1;
    int newML = -1;
    int newTL = -1;

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
                newPL = pIndex;
                newML = mIndex;
                newTL = tIndex;
                mCurrentItem = trackItem;
            } else {
                mPlaylistCore->playingIndex(&newPL, &newML, &newTL);
            }
        }
    }

    // Set playing index to playlist core
    mPlaylistCore->setPlayingIndex(newPL, newML, newTL);
    // Set playing index to models
    emit requestUpdateModelsPlayingIndex(newPL, newML, newTL);

    updateWindowTitles();
}

void TPlayerController::slotPlayButtonClicked()
{
    if(!mPlayerCore || !mPlaylistCore)
        return;

    // Try resume
    int pi = -1;
    int mi = -1;
    int ti = -1;
    if(mPlayerCore->isPaused())
    {
        if(mPlayerCore->resume()) {
            mCurrentItem = mPlaylistCore->currentTrackItem();
            updateWindowTitles();
        } else {
            mPlaylistCore->playingIndex(&pi, &mi, &ti);
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
    if(!mPlayerCore || !mMainWindow)
        return;

    mPlayerCore->pause();
    QTimer::singleShot(3000, this, SLOT(delayStopTimer()));
    mMainWindow->setButtonPlayChecked(true);
    mMainWindow->setPlayState(tr("Paused"));
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
        if(musicItem)
        {
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
}

void TPlayerController::slotStopButtonClicked()
{
    if(!mPlayerCore || !mMainWindow)
        return;

    mPlayerCore->stop();
    QTimer::singleShot(3000, this, SLOT(delayStopTimer()));
    mMainWindow->setButtonPlayChecked(true);
    mMainWindow->setPlayState(tr("Stoped"));
    mMainWindow->setProgress(0, 0);
}

void TPlayerController::slotVolumeValueChanged(float value)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_VOLUME, value);
}

void TPlayerController::slotVolumeToggled(bool toggled)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_VOLUME_ENABLE, (float)!toggled);
}

void TPlayerController::updateWindowTitles()
{
    if(!mMainWindow || !mGui)
        return;

    if(mCurrentItem)
    {
        QStringList titles;
        titles.append(mCurrentItem->displayName);
        titles.append(mCurrentItem->additionalInfo.split("\n"));
        mGui->setCaption(mCurrentItem->displayName);
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
    if(!mPlayerCore || mPlayerCore->isPaused() || mPlayerCore->isStoped())
        stopTimer();
}

void TPlayerController::resetVisualWidget()
{
    if(mMainWindow && mMainWindow->visualWidget())
        mMainWindow->visualWidget()->reset();
}

void TPlayerController::slotTimerEvent()
{
    if(!mPlayerCore || !mMainWindow)
        return;

    if(mCurrentItem)
    {
        if(mPlayerCore->isPlaying())
        {
            int playedTime = mPlayerCore->playedTime();
            int fakeDuration = mCurrentItem->duration;
            bool needCheck = false;
            if(mCurrentItem->duration <= 0)
            {
                needCheck = true;
                fakeDuration = 150000;
            }
            mMainWindow->setProgress(playedTime, fakeDuration);
            if(needCheck)
            {
                int silentMSecs = 0;
                mPlayerCore->getAudioData(ADT_SILENT_MICRO_SECONDS, &silentMSecs, NULL);
                if(silentMSecs > 3000)
                {
                    // Fix up duration manually
                    slotNextButtonClicked();                    
                }
            }
            if(fakeDuration+500 <= playedTime) {
                slotNextButtonClicked();
            }
        }

        // Update spectrum bar
        TVisualWidget *vw = mMainWindow->visualWidget();
        if(vw && !mPlayerCore->isStoped())
        {
            int size = 0;
            if(vw->spectrumMode())
            {
                TSpectrumElement *spectrumArray = NULL;
                mPlayerCore->getAudioData(ADT_SPECTRUM, (void*)&spectrumArray, (void*)&size);

                if(size>0 && spectrumArray) {
                    float levels[size];
                    for(int i = 0; i < size; i++)
                        levels[i] = spectrumArray[i].amplitude;
                    vw->setValue(levels, size);
                }
            } else {
                short *samples = NULL;
                mPlayerCore->getAudioData(ADT_SAMPLE, (void*)&samples, (void*)&size);
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
