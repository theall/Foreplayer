#ifndef TPLAYTHREAD_H
#define TPLAYTHREAD_H

#include "pch.h"

#include "front/abstractfront.h"

class TPlayThread : public QThread
{
    Q_OBJECT

public:
    explicit TPlayThread(TAbstractFront *front);
    ~TPlayThread(){ }

    void pause();
    void resume();
    void needToTerminate();

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private slots:

private:
    bool mPaused;
    bool mNeedTerminate;

    TAbstractFront *mFront;
};
#endif // TPLAYTHREAD_H
