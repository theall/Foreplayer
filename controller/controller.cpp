#include "controller.h"

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

TController::TController(QObject *parent) :
    QObject(parent),
    mThread(NULL),
    mManager(NULL),
    mMainWindow(NULL),
    mCore(NULL),
    mPlaylistController(new TPlaylistController(this))
{

}

TController::~TController()
{
    if(mThread)
    {
        mThread->terminate();
        delete mThread;
        mThread = NULL;
    }
}

void TController::joint(TGuiManager *manager, TCore *core)
{
    if(mThread)
    {
        mThread->terminate();
        delete mThread;
        mThread = NULL;
    }

    mPlaylistController->joint(manager->playlistWindow(), core->playlist());

    manager->showGui();

    mThread = new TThread(manager);
    mThread->start();

    mManager = manager;
    mCore = core;
}
