#ifndef TPLAYERCONTROLLER_H
#define TPLAYERCONTROLLER_H

#include "abstractcontroller.h"

class TPlayerController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TPlayerController(QObject *parent = 0);

    void joint(TGuiManager *gui, TCore *core);

signals:

public slots:
    void slotRequestPlay();

private:
    TPlayerCore *mPlayerCore;
    TMainWindow *mMainWindow;

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYERCONTROLLER_H
