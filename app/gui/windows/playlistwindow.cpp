#include "playlistwindow.h"

#include "preferences.h"

TPlaylistWindow::TPlaylistWindow(QWidget *parent) :
    TAbstractWindow(parent, true)
    , mToolbar(new TToolBar(this))
    , mBtnClose(new TImageButton(this))
    , mCentralWidget(new TPlaylistWidget(this))
    , mPlaylistView(mCentralWidget->playlistView())
    , mMusiclistView(mCentralWidget->musiclistView())
    , mTracklistView(mCentralWidget->tracklistView())
    , mPopmenuAddMusics(new TPopMenuAddMusics(this))
    , mPopmenuFind(new TPopMenuFind(this))
    , mPopmenuMusicList(new TPopMenuMusicList(this))
    , mPopmenuMusiclistEdit(new TPopMenuMusiclistEdit(this))
    , mPopmenuMusiclistItem(new TPopMenuMusiclistItem(this))
    , mPopmenuPlayList(new TPopMenuPlayList(this))
    , mPopmenuPlayMode(new TPopMenuPlayMode(this))
    , mPopmenuRemoveMusics(new TPopMenuRemoveMusics(this))
    , mPopmenuSort(new TPopMenuSort(this))
    , mPopmenuTrackList(new TPopMenuTrackList(this))
{
    setObjectName("PlaylistWindow");

    mPopmenuMusicList->addMenu(mPopmenuAddMusics);
    mPopmenuMusicList->addMenu(mPopmenuRemoveMusics);
    mPopmenuMusicList->addMenu(mPopmenuSort);
    mPopmenuMusicList->addMenu(mPopmenuFind);
    mPopmenuMusicList->addMenu(mPopmenuPlayList);
    mPopmenuMusicList->addMenu(mPopmenuMusiclistEdit);
    mPopmenuMusicList->addMenu(mPopmenuPlayMode);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));
    connect(mToolbar, SIGNAL(requestToggleContexMenu(TToolBar::BUTTON, QPoint, bool)), this, SLOT(slotRequestToggleButtonContexMenu(TToolBar::BUTTON, QPoint, bool)));
    connect(mPlaylistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPopupContextMenu(QPoint)));
    connect(mMusiclistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPopupContextMenu(QPoint)));
    connect(mTracklistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotPopupContextMenu(QPoint)));

    // As qt table view will process mouse move event, i have to emit it as this parent
    // window can not receive mouse move event while mouse is moving on child widget
    connect(mPlaylistView, SIGNAL(onMouseMove(QEvent*)), this, SLOT(slotOnMouseMove(QEvent*)));
    connect(mMusiclistView, SIGNAL(onMouseMove(QEvent*)), this, SLOT(slotOnMouseMove(QEvent*)));
    connect(mTracklistView, SIGNAL(onMouseMove(QEvent*)), this, SLOT(slotOnMouseMove(QEvent*)));

    connect(mPopmenuPlayList, SIGNAL(onActionAddTriggered()), this, SIGNAL(requestAddNewPlaylist()));
    connect(mPopmenuPlayList, SIGNAL(onActionRemoveTriggered()), this, SIGNAL(requestRemovePlaylist()));
    connect(mPopmenuPlayList, SIGNAL(onActionRenameTriggered()), this, SLOT(slotActionRenameTriggered()));
    connect(mPopmenuPlayList, SIGNAL(onActionSortTriggered()), this, SIGNAL(requestSortPlaylists()));
    connect(mPopmenuPlayList, SIGNAL(onActionSendTriggered()), this, SIGNAL(requestSendTo()));

    connect(mPopmenuAddMusics, SIGNAL(onActionAddMusicsTriggered()), this, SLOT(slotOnActionAddMusicsTriggered()));
    connect(mPopmenuAddMusics, SIGNAL(onActionAddDirectoryTriggered()), this, SLOT(slotOnActionAddDirectoryTriggered()));

    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveSelectionsTriggered()), this, SLOT(slotOnActionRemoveSelectionsTriggered()));
    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveRedundantTriggered()), this, SLOT(slotOnActionRemoveRedundantTriggered()));
    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveErrorsTriggered()), this, SLOT(slotOnActionRemoveErrorsTriggered()));
    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveAllTriggered()), this, SLOT(slotOnActionRemoveAllTriggered()));

    retranslateUi();
}

void TPlaylistWindow::on_btnClose_clicked()
{
    hide();

    emit playlistWindowToggled(false);
}

void TPlaylistWindow::slotRequestToggleButtonContexMenu(TToolBar::BUTTON id, QPoint pos, bool checked)
{
    Q_UNUSED(checked)

    TAbstractPopMenu *menu = NULL;
    if (id==TToolBar::BTN_ADD)
         menu = mPopmenuAddMusics;
    else if (id==TToolBar::BTN_DELETE)
         menu = mPopmenuRemoveMusics;
    else if (id==TToolBar::BTN_PLAYLIST)
         menu = mPopmenuPlayList;
    else if (id==TToolBar::BTN_SORT)
         menu = mPopmenuSort;
    else if (id==TToolBar::BTN_FIND)
         menu = mPopmenuFind;
    else if (id==TToolBar::BTN_EDIT)
         menu = mPopmenuMusiclistEdit;
    else if (id==TToolBar::BTN_MODE)
         menu = mPopmenuPlayMode;

    if(menu)
    {
        QPoint pt = mToolbar->mapToGlobal(pos);
        pt.setY(pt.y() - menu->sizeHint().height());
        menu->popup(pt);
    }
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

void TPlaylistWindow::slotOnMouseMove(QEvent *event)
{
    setCursor(Qt::ArrowCursor);
    event->accept();
}

void TPlaylistWindow::slotActionRenameTriggered()
{
    if(!mPlaylistView)
        return;

    mPlaylistView->editCurrent();
}

void TPlaylistWindow::slotOnActionAddMusicsTriggered()
{
    TPreferences *prefs = TPreferences::instance();

    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            tr("Add one or more musics files to current playlist"),
                            prefs->lastOpenDialogPath(),
                            tr("Music files (*.mp3 *.wav *.wma);All files (*.*)"));

    tryAddMusicFiles(files);
}

void TPlaylistWindow::slotOnActionAddDirectoryTriggered()
{
    TPreferences *prefs = TPreferences::instance();

    QString path = QFileDialog::getExistingDirectory(
                            this,
                            tr("Choose directory"),
                            prefs->lastOpenDirectory());

    QDir dir(path);
    if(!dir.isReadable())
        return;

    tryAddMusicFiles(dir.entryList(QDir::Files));
}

void TPlaylistWindow::slotOnActionRemoveSelectionsTriggered()
{
    if(!mMusiclistView)
        return;

    QList<int> selected = mMusiclistView->selectedRows().toList();
    if(selected.size() > 0)
        emit requestRemoveSelections(selected);
}

void TPlaylistWindow::slotOnActionRemoveRedundantTriggered()
{
    if(!mMusiclistView)
        return;

    emit requestRemoveRedundant();
}

void TPlaylistWindow::slotOnActionRemoveErrorsTriggered()
{
    if(!mMusiclistView)
        return;

    emit requestRemoveErrors();
}

void TPlaylistWindow::slotOnActionRemoveAllTriggered()
{
    if(!mMusiclistView)
        return;

    emit requestRemoveAll();
}

void TPlaylistWindow::tryAddMusicFiles(QStringList files)
{
    if(!mMusiclistView)
        return;

    mMusiclistView->addFiles(files);
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
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnClose->loadFromSkin(element.firstChildElement(TAG_PLAYLIST_CLOSE), skin);
    mToolbar->loadFromSkin(element.firstChildElement(TAG_PLAYLIST_TOOLBAR), skin);
    mCentralWidget->loadFromSkin(element.firstChildElement(TAG_PLAYLIST_PLAYLIST), skin);

    QDomElement scrollBarElement = element.firstChildElement(TAG_PLAYLIST_SCROLLBAR);

    TScrollBar::setPixmaps(
                skin->findPixmap(scrollBarElement.attribute(ATTR_BAR_IMAGE)),
                skin->findPixmap(scrollBarElement.attribute(ATTR_BUTTONS_IMAGE)),
                skin->findPixmap(scrollBarElement.attribute(ATTR_THUMB_IMAGE))
                );
}
