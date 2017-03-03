#include "abstractcontroller.h"

TAbstractController::TAbstractController(QObject *parent) :
    QObject(parent),
    mTimer(new QTimer(this))
{
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotTimerEvent()));
}

TAbstractController::~TAbstractController()
{
    if(mTimer)
    {
        mTimer->stop();
        delete mTimer;
    }
}

void TAbstractController::joint(TGuiManager *manager, TCore *core)
{
    mGui = manager;
    mCore = core;
}

void TAbstractController::startTimer(int mSecs)
{
    if(mTimer && !mTimer->isActive())
        mTimer->start(mSecs);
}

void TAbstractController::stopTimer()
{
    if(mTimer && mTimer->isActive())
        mTimer->stop();
}

