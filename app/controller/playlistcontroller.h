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

signals:
    void requestPlay(int pIndex, int mIndex, int tIndex);

private slots:
    //// From gui
    void slotPlaylistIndexChanged(int index);
    void slotMusiclistIndexChanged(int index);
    void slotTracklistIndexChanged(int index);

    void slotPlaylistItemSelected(int index);
    void slotMusiclistItemSelected(int index);
    void slotTracklistItemSelected(int index);

    void slotRequestMovePlaylists(QList<int> indexes, int pos, QList<int> &newIndexes);
    void slotRequestMoveMusics(QList<int> indexes, int pos, QList<int> &newIndexes);
    void slotRequestMoveTracks(QList<int> indexes, int pos, QList<int> &newIndexes);

    //// Play list
    void slotRequestAddNewPlaylist();
    void slotRequestRemovePlaylist();
    void slotRequestRenamePlaylist();
    void slotRequestSortPlaylists();
    void slotRequestSendTo();

    //// Music list
    void slotRequestAddMusicFiles(QStringList files, int pos, QList<int> &newIndexes);
    void slotRequestRemoveSelections(QList<int> indexes);
    void slotRequestRemoveRedundant();
    void slotRequestRemoveErrors();
    void slotRequestRemoveAll();

    // Music item
    void slotRequestReparseMusicItem(int row);
    void slotRequestPlayMusicItem(int row);
    void slotRequestCopyMusicItem(QSet<int> rows);
    void slotRequestCutMusicItem(QSet<int> rows);
    void slotRequestPasteMusicItem(int pos);
    void slotRequestDeleteMusicItem(QSet<int> rows);
    void slotRequestRenameMusicItem(int row);
    void slotRequestExplorerMusicItem(int row);
    void slotRequestExportMusicItem(int row);
    void slotRequestDetailMusicItem(int row);

    //// From controller
    void slotRequestUpdateModelsPlayingIndex(int pi, int mi, int ti);
    void slotRequestCurrentIndex(int *pIndex, int *mIndex, int *tIndex);

protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;

private:
    TPlaylistModel *mPlaylistModel;
    TMusiclistModel *mMusiclistModel;
    TTrackListModel *mTracklistModel;

    TPlaylistWindow *mPlaylistWindow;
    TPlaylistWidget *mPlaylistWidget;
    TPlaylistView *mPlaylistView;
    TMusiclistView *mMusiclistView;
    TTracklistView *mTracklistView;
    TPlaylistCore *mPlaylistCore;
};

#endif // TPLAYLISTCONTROLLER_H
