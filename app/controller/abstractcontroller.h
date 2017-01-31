#ifndef TABSTRACTCONTROLLER_H
#define TABSTRACTCONTROLLER_H

#include "../core/core.h"
#include "../gui/guimanager.h"

class TAbstractController : public QObject
{
    Q_OBJECT
public:
    explicit TAbstractController(QObject *parent = 0);
    ~TAbstractController();

    virtual void joint(TGuiManager *manager, TCore *core);

    void startTimer(int mSecs = 100);
    void stopTimer();

protected slots:
    virtual void slotTimerEvent() = 0;

signals:

protected:
    TGuiManager *mGui;
    TCore *mCore;
    QTimer *mTimer;
};

#endif // TABSTRACTCONTROLLER_H
