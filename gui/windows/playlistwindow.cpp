#include "playlistwindow.h"

TPlaylistWindow::TPlaylistWindow(QWidget *parent) :
    TAbstractWindow(parent, true)
    , mToolbar(new TToolBar(this))
    , mBtnClose(new TImageButton(this))
    , mCentralWidget(new TPlaylistWidget(this))
    , mPopmenuAddMusics(new TPopMenuAddMusics(this))
    , mPopmenuFind(new TPopMenuFind(this))
    , mPopmenuMusicList(new TPopMenuMusicList(this))
    , mPopmenuMusiclistItem(new TPopMenuMusiclistItem(this))
    , mPopmenuPlayList(new TPopMenuPlayList(this))
    , mPopmenuPlayMode(new TPopMenuPlayMode(this))
    , mPopmenuRemoveMusics(new TPopMenuRemoveMusics(this))
    , mPopmenuSort(new TPopMenuSort(this))
    , mPopmenuTrackList(new TPopMenuTrackList(this))
{
    setObjectName("PlaylistWindow");

    setMouseTracking(true);

    mPopmenuMusicList->addMenu(mPopmenuAddMusics);
    mPopmenuMusicList->addMenu(mPopmenuRemoveMusics);
    mPopmenuMusicList->addMenu(mPopmenuSort);
    mPopmenuMusicList->addMenu(mPopmenuFind);
    mPopmenuMusicList->addMenu(mPopmenuPlayList);
    mPopmenuMusicList->addMenu(mPopmenuPlayMode);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));
    connect(mToolbar, SIGNAL(buttonClicked(TToolBar::BUTTON, QPoint)), this, SLOT(slotToolbarClicked(TToolBar::BUTTON, QPoint)));
    connect(mToolbar, SIGNAL(mouseLeave(TToolBar::BUTTON)), this, SLOT(slotToolbarButtonMouseLeave(TToolBar::BUTTON)));
    connect(mCentralWidget->playlistView(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPopupContextMenu(QPoint)));
    connect(mCentralWidget->musiclistView(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPopupContextMenu(QPoint)));
    connect(mCentralWidget->tracklistView(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPopupContextMenu(QPoint)));

    retranslateUi();
}

void TPlaylistWindow::on_btnClose_clicked()
{
    hide();

    emit playlistWindowToggled(false);
}

void TPlaylistWindow::slotToolbarClicked(TToolBar::BUTTON id, QPoint pos)
{
    QPoint pt = mToolbar->pos();
    pos.setY(pt.y());
    if(id==TToolBar::BTN_ADD)
    {

    }
}

void TPlaylistWindow::slotToolbarButtonMouseLeave(TToolBar::BUTTON id)
{
    Q_UNUSED(id)
}

void TPlaylistWindow::slotPopupContextMenu(QPoint pos)
{
    if(TPlaylistView *plalistView = dynamic_cast<TPlaylistView*>(sender())) {
        mPopmenuPlayList->pop(plalistView->mapToGlobal(pos));
    } else if (TMusiclistView *musiclistView = dynamic_cast<TMusiclistView*>(sender())) {
        mPopmenuMusicList->popup(musiclistView->mapToGlobal(pos));
    } else if (TTracklistView *tracklistView = dynamic_cast<TTracklistView*>(sender())) {
        mPopmenuTrackList->popup(tracklistView->mapToGlobal(pos));
    }
}

void TPlaylistWindow::retranslateUi()
{
    mBtnClose->setToolTip(tr("Close"));
    mToolbar->retranslateUi();
}

void TPlaylistWindow::resizeEvent(QResizeEvent *event)
{
    if(mCentralWidget->updatePosition(event->size()))
    {
        TAbstractWindow::resizeEvent(event);

        mBtnClose->updatePos();
        mToolbar->updatePos();
    }
}

void TPlaylistWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    TAbstractWindow::loadFromSkin(element, skin);

    mBtnClose->loadFromSkin(element.firstChildElement(TAG_PLAYLIST_CLOSE), skin);
    mToolbar->loadFromSkin(element.firstChildElement(TAG_PLAYLIST_TOOLBAR), skin);
    mCentralWidget->loadFromSkin(element.firstChildElement(TAG_PLAYLIST_PLAYLIST), skin);
}
