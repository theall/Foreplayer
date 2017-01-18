#include "abstractcontroller.h"

TAbstractController::TAbstractController(QObject *parent) :
    QObject(parent),
    mTimer(new QTimer(this))
{
    mTimer->setInterval(1000);
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

void TAbstractController::startTimer()
{
    if(mTimer)
        mTimer->start();
}

void TAbstractController::stopTimer()
{
    if(mTimer)
        mTimer->stop();
}

