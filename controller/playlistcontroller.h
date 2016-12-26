#ifndef TPLAYLISTCONTROLLER_H
#define TPLAYLISTCONTROLLER_H

#include "abstractcontroller.h"

#include "model/playlistmodel.h"
#include "model/musiclistmodel.h"
#include "model/tracklistmodel.h"

class TPlaylistController : public TAbstractController
{
    Q_OBJECT

public:
    explicit TPlaylistController(QObject *parent = 0);

    void joint(TGuiManager *manager, TCore *core) Q_DECL_OVERRIDE;

private:
    TPlaylistModel *mPlaylistModel;
    TMusiclistModel *mMusiclistModel;
    TTrackListModel *mTracklistModel;

    TPlaylistWidget *mPlaylistWidget;
    TPlaylistCore *mPlaylistCore;

private slots:
    void slotPlaylistIndexChanged(int index);
    void slotMusiclistIndexChanged(int index);
    void slotTracklistIndexChanged(int index);
    void slotRequestSetColors(QFont font,
                              QColor textColor,
                              QColor selectedTextColor,
                              QColor numberColor,
                              QColor durationColor,
                              QColor currentRowTextColor,
                              QColor backgroundColor);

protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTCONTROLLER_H
