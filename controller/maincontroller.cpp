#include "maincontroller.h"

TThread::TThread(TGuiManager *manager):
    QThread()
{
    mManager = manager;
    mPlaying = false;
}

void TThread::run()
{
    QVector<int> samples;

    TMainWindow *mainWindow = mManager->mainWindow();
    TVisualWidget *visualWidget = mainWindow->visualWidget();
    int sampleSize = visualWidget->sampleSize();
    int maxValue = visualWidget->maxValue();
    samples.resize(sampleSize);

    connect(mainWindow, SIGNAL(playClicked()), this, SLOT(playClicked()));
    connect(mainWindow, SIGNAL(pauseClicked()), this, SLOT(pauseClicked()));

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
    mManager(NULL),
    mMainWindow(NULL),
    mCore(NULL),
    mPlaylistController(new TPlaylistController(this))
{

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
    if(mThread)
    {
        mThread->terminate();
        delete mThread;
        mThread = NULL;
    }

    mPlaylistController->joint(manager, core);

    manager->showGui();

    mThread = new TThread(manager);
    mThread->start();

    TAbstractController::joint(manager, core);
}

void TMainController::slotTimerEvent()
{
}
