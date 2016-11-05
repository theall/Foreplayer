#ifndef TPLAYLISTCONTROLLER_H
#define TPLAYLISTCONTROLLER_H

#include "abstractcontroller.h"

class TPlaylistController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TPlaylistController(QObject *parent = 0);

    void joint(TGuiManager *manager, TCore *core) Q_DECL_OVERRIDE;

private:
    TPlaylistWindow *mGui;
    TPlaylistCore *mCore;

protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTCONTROLLER_H
