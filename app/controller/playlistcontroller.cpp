#include "playlistcontroller.h"

TPlaylistController::TPlaylistController(QObject *parent) :
    TAbstractController(parent),
    mPlaylistModel(new TPlaylistModel(this)),
    mMusiclistModel(new TMusiclistModel(this)),
    mTracklistModel(new TTrackListModel(this)),
    mPlaylistWindow(NULL),
    mPlaylistWidget(NULL),
    mPlaylistView(NULL),
    mMusiclistView(NULL),
    mTracklistView(NULL),
    mPlaylistCore(NULL)
{

}

void TPlaylistController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    TAbstractController::joint(gui, core);

    mPlaylistWindow = gui->playlistWindow();
    Q_ASSERT(mPlaylistWindow);

    mPlaylistWidget = gui->playlistWindow()->playlistWidget();
    Q_ASSERT(mPlaylistWidget);

    mPlaylistView = mPlaylistWidget->playlistView();
    mMusiclistView = mPlaylistWidget->musiclistView();
    mTracklistView = mPlaylistWidget->tracklistView();

    TAbstractModel::setColorParameters(
        mPlaylistWidget->textFont(),
        mPlaylistWidget->colorText(),
        mPlaylistWidget->colorHilight(),
        mPlaylistWidget->colorNumber(),
        mPlaylistWidget->colorDuration(),
        mPlaylistWidget->colorSelect(),
        mPlaylistWidget->colorBkgnd(),
        mPlaylistWidget->colorBkgnd2());

    mPlaylistCore = core->playlist();
    mPlaylistModel->setPlaylistCore(mPlaylistCore);
    mMusiclistModel->setPlaylistCore(mPlaylistCore);
    mTracklistModel->setPlaylistCore(mPlaylistCore);

    mPlaylistView->setModel(mPlaylistModel);
    mMusiclistView->setModel(mMusiclistModel);
    mTracklistView->setModel(mTracklistModel);

    connect(mPlaylistView, SIGNAL(onCurrentRowChanged(int)), this, SLOT(slotPlaylistIndexChanged(int)));
    connect(mMusiclistView, SIGNAL(onCurrentRowChanged(int)), this, SLOT(slotMusiclistIndexChanged(int)));
    connect(mTracklistView, SIGNAL(onCurrentRowChanged(int)), this, SLOT(slotTracklistIndexChanged(int)));

    connect(mPlaylistView, SIGNAL(onDoubleClickItem(int)), this, SLOT(slotPlaylistItemSelected(int)));
    connect(mMusiclistView, SIGNAL(onDoubleClickItem(int)), this, SLOT(slotMusiclistItemSelected(int)));
    connect(mTracklistView, SIGNAL(onDoubleClickItem(int)), this, SLOT(slotTracklistItemSelected(int)));

    // Play list
    connect(mPlaylistWindow, SIGNAL(requestAddNewPlaylist()), this, SLOT(slotRequestAddNewPlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestRemovePlaylist()), this, SLOT(slotRequestRemovePlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestRenamePlaylist()), this, SLOT(slotRequestRenamePlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestSortPlaylists()), this, SLOT(slotRequestSortPlaylists()));
    connect(mPlaylistWindow, SIGNAL(requestSendTo()), this, SLOT(slotRequestSendTo()));

    // Music list
    connect(mPlaylistWindow, SIGNAL(requestRemoveSelections(QList<int>)), this, SLOT(slotRequestRemoveSelections(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestRemoveRedundant()), this, SLOT(slotRequestRemoveRedundant()));
    connect(mPlaylistWindow, SIGNAL(requestRemoveErrors()), this, SLOT(slotRequestRemoveErrors()));
    connect(mPlaylistWindow, SIGNAL(requestRemoveAll()), this, SLOT(slotRequestRemoveAll()));

    connect(mPlaylistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMovePlaylists(QList<int>, int, QList<int>&)));
    connect(mMusiclistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMoveMusics(QList<int>, int, QList<int>&)));
    connect(mMusiclistView, SIGNAL(requestAddFiles(QStringList,int,QList<int>&)), this, SLOT(slotRequestAddMusicFiles(QStringList, int, QList<int>&)));

    connect(mTracklistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMoveTracks(QList<int>, int, QList<int>&)));

    //mPlaylistView->selectRow(mPlaylistCore->currentPlaylistIndex());
    int playingPlaylistIndex = mPlaylistCore->playingPlaylistIndex();
    mPlaylistModel->setCurrentIndex(playingPlaylistIndex);
    slotPlaylistIndexChanged(playingPlaylistIndex);
}

void TPlaylistController::slotPlaylistIndexChanged(int index)
{
    if(!mMusiclistModel || !mPlaylistModel || !mPlaylistCore)
        return;

    mPlaylistModel->setCurrentIndex(index);

    TPlaylistItem *item = mPlaylistCore->playlistItem(index);
    mMusiclistModel->setPlayListItem(item);

    // Check if is playing index as the playing index in models will reset after every set item
    if(index == mPlaylistCore->playingPlaylistIndex())
    {
        mMusiclistModel->setPlayingIndex(mPlaylistCore->playingMusicIndex());
    }

    // Change track list model while playlist index changed
    int musicIndex = 0;
    if(index == mPlaylistCore->playingPlaylistIndex())
    {
        // Is current playing playlist
        musicIndex = mPlaylistCore->playingMusicIndex();
    }
    mMusiclistModel->setCurrentIndex(musicIndex);
    slotMusiclistIndexChanged(musicIndex);
}

void TPlaylistController::slotMusiclistIndexChanged(int index)
{
    if(!mPlaylistCore || !mTracklistModel)
        return;

    TPlaylistItem *playlistItem = mPlaylistCore->playlistItem(mPlaylistModel->currentIndex());
    if(playlistItem)
    {
        TMusicItem *musicItem = playlistItem->musicItem(index);
        mTracklistModel->setMusicItem(musicItem);

        // Check if this music item is playing
        if(mPlaylistCore->currentMusicItem()==musicItem)
            mTracklistModel->setPlayingIndex(mPlaylistCore->playingTrackIndex());
    }
}

void TPlaylistController::slotTracklistIndexChanged(int index)
{
    Q_UNUSED(index);
}

void TPlaylistController::slotPlaylistItemSelected(int index)
{
    Q_UNUSED(index);
}

void TPlaylistController::slotMusiclistItemSelected(int index)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->setCurrentIndex(index);
    slotTracklistItemSelected(0);
}

void TPlaylistController::slotTracklistItemSelected(int index)
{
    if(!mPlaylistModel || !mMusiclistModel || !mTracklistModel)
        return;

    mTracklistModel->setCurrentIndex(index);

    // Play current track
    emit requestPlay(
                mPlaylistModel->currentIndex(),
                mMusiclistModel->currentIndex(),
                mTracklistModel->currentIndex());
}

void TPlaylistController::slotRequestAddNewPlaylist()
{
    if(!mPlaylistModel || !mPlaylistView)
        return;

    mPlaylistModel->add(tr("New playlist"));
}

void TPlaylistController::slotRequestRemovePlaylist()
{
    if(!mPlaylistModel || !mPlaylistView)
        return;

    int currentIndex = mPlaylistModel->currentIndex();
    mPlaylistModel->remove(currentIndex);

    // Reget current index after item removed
    currentIndex = mPlaylistModel->currentIndex();
    if(currentIndex > -1)
        mPlaylistView->selectRow(currentIndex);
}

void TPlaylistController::slotRequestRenamePlaylist()
{
    if(!mPlaylistModel || !mPlaylistView)
        return;

    mPlaylistView->edit(mPlaylistView->currentIndex());
}

void TPlaylistController::slotRequestSortPlaylists()
{
    if(!mPlaylistModel)
        return;

    mPlaylistModel->sort();
}

void TPlaylistController::slotRequestSendTo()
{

}

void TPlaylistController::slotRequestMovePlaylists(QList<int> indexes, int pos, QList<int> &newIndexes)
{
    if(!mPlaylistModel)
        return;

    mPlaylistModel->moveItems(indexes, pos, newIndexes);
}

void TPlaylistController::slotRequestMoveMusics(QList<int> indexes, int pos, QList<int> &newIndexes)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->moveItems(indexes, pos, newIndexes);
}

void TPlaylistController::slotRequestMoveTracks(QList<int> indexes, int pos, QList<int> &newIndexes)
{
    if(!mTracklistModel)
        return;

    mTracklistModel->moveItems(indexes, pos, newIndexes);
}

void TPlaylistController::slotRequestAddMusicFiles(QStringList files, int pos, QList<int> &newIndexes)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->insertFiles(files, pos, newIndexes);
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->musicItem(mMusiclistModel->currentIndex()));
}

void TPlaylistController::slotRequestRemoveSelections(QList<int> indexes)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeSelections(indexes);
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->musicItem(mMusiclistModel->currentIndex()));
}

void TPlaylistController::slotRequestRemoveRedundant()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeRedundant();
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->musicItem(mMusiclistModel->currentIndex()));
}

void TPlaylistController::slotRequestRemoveErrors()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeErrors();
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->musicItem(mMusiclistModel->currentIndex()));
}

void TPlaylistController::slotRequestRemoveAll()
{
    if(!mMusiclistModel || !mTracklistModel)
        return;

    mTracklistModel->setMusicItem(NULL);
    mMusiclistModel->removeAll();
}

void TPlaylistController::slotRequestUpdateModelsPlayingIndex(int pi, int mi, int ti)
{
    if(mPlaylistModel)
        mPlaylistModel->setPlayingIndex(pi);

    if(mMusiclistModel)
        mMusiclistModel->setPlayingIndex(mi);

    if(mTracklistModel)
        mTracklistModel->setPlayingIndex(ti);
}

void TPlaylistController::slotTimerEvent()
{
}
