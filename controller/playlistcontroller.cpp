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

    connect(mPlaylistWindow, SIGNAL(requestAddNewPlaylist()), this, SLOT(slotRequestAddNewPlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestRemovePlaylist()), this, SLOT(slotRequestRemovePlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestRenamePlaylist()), this, SLOT(slotRequestRenamePlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestSortPlaylists()), this, SLOT(slotRequestSortPlaylists()));
    connect(mPlaylistWindow, SIGNAL(requestSendTo()), this, SLOT(slotRequestSendTo()));

    connect(mPlaylistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMovePlaylists(QList<int>, int, QList<int>&)));
    TAbstractController::joint(gui, core);
}

void TPlaylistController::slotPlaylistIndexChanged(int index)
{
    if(mPlaylistModel)
    {
        mPlaylistModel->setCurrentIndex(index);
    }
}

void TPlaylistController::slotMusiclistIndexChanged(int index)
{
    if(mMusiclistModel)
    {
        mMusiclistModel->setCurrentIndex(index);
    }
}

void TPlaylistController::slotTracklistIndexChanged(int index)
{
    if(mTracklistModel)
    {
        mTracklistModel->setCurrentIndex(index);
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

void TPlaylistController::slotRequestMovePlaylists(QList<int> indexes, int insertPos, QList<int> &newIndexes)
{
    if(!mPlaylistModel)
        return;
    mPlaylistModel->moveItems(indexes, insertPos, newIndexes);
}

void TPlaylistController::slotTimerEvent()
{
}
