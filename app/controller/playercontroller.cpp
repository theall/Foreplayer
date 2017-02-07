#include "playercontroller.h"

TPlayerController::TPlayerController(QObject *parent) :
    TAbstractController(parent)
  , mCurrentItem(NULL)
  , mSpectrum(new TSpectrumAnalyser(this))
{

}

TPlayerController::~TPlayerController()
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

    qRegisterMetaType<TFrequencySpectrum>("TFrequencySpectrum");
    qRegisterMetaType<WindowFunction>("WindowFunction");

    connect(mSpectrum,
            SIGNAL(spectrumChanged(TFrequencySpectrum)),
            this,
            SLOT(slotSpectrumChanged(TFrequencySpectrum)));
}

void TPlayerController::slotRequestPlay(int pIndex, int mIndex, int tIndex)
{
    if(!mPlaylistCore || !mPlayerCore || !mMainWindow)
        return;

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
    stopTimer();
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

void TPlayerController::slotSpectrumChanged(const TFrequencySpectrum &spectrum)
{
    Q_UNUSED(spectrum);
}

void TPlayerController::updateWindowTitles()
{
    if(mCurrentItem)
    {
        QStringList titles;
        titles.append(mCurrentItem->displayName);
        titles.append(mCurrentItem->additionalInfo.split("\n"));
        mMainWindow->setCaption(mCurrentItem->displayName);
        mMainWindow->setTitles(titles);
        mMainWindow->setPlayState(tr("Playing"));
        mMainWindow->setButtonPlayChecked(false);
        startTimer();
    } else {
        stopTimer();
        mMainWindow->setPlayState(tr("Stoped"));
        mMainWindow->setButtonPlayChecked(true);
    }
}

void TPlayerController::slotTimerEvent()
{
    if(!mPlayerCore || !mMainWindow)
        return;

    if(mCurrentItem)
    {
        int playedTime = mPlayerCore->playedTime();
        mMainWindow->setProgress(playedTime, mCurrentItem->duration);
        if(mCurrentItem->duration+500 <= playedTime)
        {
            slotNextButtonClicked();
        }

        // Update spectrum bar
        int size = 0;
        short *samples = NULL;
        mPlayerCore->currentSamples(&size, &samples);
        if(size > 0) {
            QByteArray buffer;
            short *pSample = samples;
            for(int i=0;i<size;i++)
            {
                short sample = ~*pSample;
                buffer.append((char)((sample&0xff00)>>16));
                buffer.append((char)sample&0xff);
                //buffer.append((char*)&sample, 2);
                pSample++;
            }
            //mSpectrum->calculate(buffer);
            mMainWindow->visualWidget()->setValue(buffer);
        }
    } else {
        mMainWindow->setProgress(0, 0);
    }
}
