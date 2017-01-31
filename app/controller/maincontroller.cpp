#include "maincontroller.h"

TThread::TThread(TGuiManager *manager):
    QThread()
{
    mManager = manager;
    mPlaying = false;
}

void TThread::run()
{
    QVector<byte> samples;

    TMainWindow *mainWindow = mManager->mainWindow();
    TVisualWidget *visualWidget = mainWindow->visualWidget();
    int sampleSize = visualWidget->sampleSize();
    int maxValue = visualWidget->maxValue();
    samples.resize(sampleSize);

    while(1)
    {
        if(mPlaying)
        {
            for(int i=0;i<sampleSize;i++)
            {
                float x = qrand();
                x /= RAND_MAX;
                x *= maxValue;

                samples[i] = x;
            }
            visualWidget->setValue(samples);
        }

        msleep(100);
    }
}

TMainController::TMainController(QObject *parent) :
    TAbstractController(parent),
    mThread(NULL),
    mPlayerController(new TPlayerController(this)),
    mPlaylistController(new TPlaylistController(this))
{
    connect(mPlaylistController,
            SIGNAL(requestPlay(int,int,int)),
            mPlayerController,
            SLOT(slotRequestPlay(int,int,int)));
    connect(mPlayerController,
            SIGNAL(requestUpdateModelsPlayingIndex(int,int,int)),
            mPlaylistController,
            SLOT(slotRequestUpdateModelsPlayingIndex(int,int,int)));
}

TMainController::~TMainController()
{
    if(mThread)
    {
        mThread->terminate();
        delete mThread;
        mThread = NULL;
    }
}

void TMainController::joint(TGuiManager *manager, TCore *core)
{
    Q_ASSERT(manager);
    Q_ASSERT(core);

    if(mThread)
    {
        mThread->terminate();
        delete mThread;
        mThread = NULL;
    }

    connect(manager, SIGNAL(requestShutdown()), this, SLOT(slotQuitApp()));

    if(manager->loadSkin("z:/skins/fulkfour.zip"))
    {
        mPlayerController->joint(manager, core);
        mPlaylistController->joint(manager, core);

        manager->showGui();
    } else {

    }

    mThread = new TThread(manager);
    mThread->start();

    TAbstractController::joint(manager, core);
}

void TMainController::slotQuitApp()
{
    qApp->quit();
}

void TMainController::slotTimerEvent()
{
}
