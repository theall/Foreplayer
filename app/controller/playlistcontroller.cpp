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

    connect(mPlaylistWidget, SIGNAL(onPlaylistIndexChanged(int)), this, SLOT(slotPlaylistIndexChanged(int)));
    connect(mPlaylistWidget, SIGNAL(onMusiclistIndexChanged(int)), this, SLOT(slotMusiclistIndexChanged(int)));
    connect(mPlaylistWidget, SIGNAL(onTracklistIndexChanged(int)), this, SLOT(slotTracklistIndexChanged(int)));

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

    slotPlaylistIndexChanged(mPlaylistCore->currentIndex());
}

void TPlaylistController::slotPlaylistIndexChanged(int index)
{
    if(!mPlaylistModel || !mMusiclistModel || !mPlaylistCore)
        return;

    mPlaylistModel->setCurrentIndex(index);
    TPlaylistItem *item = mPlaylistCore->currentPlaylistItem();
    mMusiclistModel->setPlayListItem(item);
    slotMusiclistIndexChanged(item->currentIndex());
}

void TPlaylistController::slotMusiclistIndexChanged(int index)
{
    if(mMusiclistModel || !mTracklistModel)
    {
        mMusiclistModel->setCurrentIndex(index);
        mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->currentItem());
        slotTracklistIndexChanged(0);
    }
}

void TPlaylistController::slotTracklistIndexChanged(int index)
{
    if(mTracklistModel && mPlaylistCore)
    {
        mTracklistModel->setCurrentIndex(index);

        // Play current track
        emit requestPlay();
    }
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
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->currentItem());
}

void TPlaylistController::slotRequestRemoveSelections(QList<int> indexes)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeSelections(indexes);
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->currentItem());
}

void TPlaylistController::slotRequestRemoveRedundant()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeRedundant();
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->currentItem());
}

void TPlaylistController::slotRequestRemoveErrors()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeErrors();
    mTracklistModel->setMusicItem(mPlaylistCore->currentPlaylistItem()->currentItem());
}

void TPlaylistController::slotRequestRemoveAll()
{
    if(!mMusiclistModel || !mTracklistModel)
        return;

    mTracklistModel->setMusicItem(NULL);
    mMusiclistModel->removeAll();
}

void TPlaylistController::slotTimerEvent()
{
}
