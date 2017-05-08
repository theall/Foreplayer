/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "playlistwindow.h"

#include "preferences.h"
#include <QMessageBox>

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
    connect(mPopmenuPlayList, SIGNAL(onActionRemoveTriggered()), this, SLOT(slotActionRemovePlaylist()));
    connect(mPopmenuPlayList, SIGNAL(onActionRenameTriggered()), this, SLOT(slotActionRenameTriggered()));
    connect(mPopmenuPlayList, SIGNAL(onActionSortTriggered()), this, SIGNAL(requestSortPlaylists()));
    connect(mPopmenuPlayList, SIGNAL(onActionSendTriggered()), this, SIGNAL(requestSendTo()));

    connect(mPopmenuAddMusics, SIGNAL(onActionAddMusicsTriggered()), this, SLOT(slotOnActionAddMusicsTriggered()));
    connect(mPopmenuAddMusics, SIGNAL(onActionAddDirectoryTriggered()), this, SLOT(slotOnActionAddDirectoryTriggered()));

    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveSelectionsTriggered()), this, SLOT(slotOnActionRemoveSelectionsTriggered()));
    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveRedundantTriggered()), this, SLOT(slotOnActionRemoveRedundantTriggered()));
    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveErrorsTriggered()), this, SLOT(slotOnActionRemoveErrorsTriggered()));
    connect(mPopmenuRemoveMusics, SIGNAL(onActionRemoveAllTriggered()), this, SLOT(slotOnActionRemoveAllTriggered()));

    // Music item popup menu
    connect(mPopmenuMusiclistItem, SIGNAL(onActionReparseTriggered()), this, SLOT(slotReparseMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionPlayTriggered()), this, SLOT(slotPlayMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionCopyTriggered()), this, SLOT(slotCopyMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionCutTriggered()), this, SLOT(slotCutMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionPasteTriggered()), this, SLOT(slotPasteMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionDeleteTriggered()), this, SLOT(slotDeleteMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionRenameTriggered()), this, SLOT(slotRenameMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionExplorerTriggered()), this, SLOT(slotExplorerMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionExportTriggered()), this, SLOT(slotExportMusicItemTriggered()));
    connect(mPopmenuMusiclistItem, SIGNAL(onActionViewTriggered()), this, SLOT(slotViewMusicItemTriggered()));

    // Track list popup menu
    connect(mPopmenuTrackList, SIGNAL(onActionPlayTriggered()), this, SLOT(slotPlayTrackItemTriggered()));
    connect(mPopmenuTrackList, SIGNAL(onActionCopyTriggered()), this, SLOT(slotCopyTrackItemTriggered()));
    connect(mPopmenuTrackList, SIGNAL(onActionChangeTriggered()), this, SLOT(slotChangeTrackItemTriggered()));
    connect(mPopmenuTrackList, SIGNAL(onActionExportTriggered()), this, SLOT(slotExportTrackItemTriggered()));
    connect(mPopmenuTrackList, SIGNAL(onActionViewTriggered()), this, SLOT(slotViewTrackItemTriggered()));

    // Sort music item menu
    connect(mPopmenuSort, SIGNAL(onActionSortAsNameTriggered()), this, SIGNAL(requestSortMusiclistAsName()));
    connect(mPopmenuSort, SIGNAL(onActionSortAsArtistTriggered()), this, SIGNAL(requestSortMusiclistAsArtist()));
    connect(mPopmenuSort, SIGNAL(onActionSortAsAlbumTriggered()), this, SIGNAL(requestSortMusiclistAsAlbum()));
    connect(mPopmenuSort, SIGNAL(onActionSortAsDurationTriggered()), this, SIGNAL(requestSortMusiclistAsDuration()));
    connect(mPopmenuSort, SIGNAL(onActionSortRandomTriggered()), this, SIGNAL(requestSortMusiclistRandom()));
    connect(mPopmenuSort, SIGNAL(onActionSortReverseTriggered()), this, SIGNAL(requestSortMusiclistReverse()));

    mToolbar->setVisible(false);
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
        if(mToolbar->mapToParent(pos).y() > height()/2)
            pt.setY(pt.y() - menu->sizeHint().height());
        else
            pt.setY(pt.y() + mToolbar->height());
        menu->popup(pt);
    }
}

void TPlaylistWindow::slotPopupContextMenu(QPoint pos)
{
    if(TPlaylistView *playlistView = dynamic_cast<TPlaylistView*>(sender())) {
        mPopmenuPlayList->display(playlistView->mapToGlobal(pos), playlistView->selectedRows().size());
    } else if (TMusiclistView *musiclistView = dynamic_cast<TMusiclistView*>(sender())) {
        mPopmenuMusiclistItem->display(musiclistView->mapToGlobal(pos), musiclistView->selectedRows().size());
    } else if (TTracklistView *tracklistView = dynamic_cast<TTracklistView*>(sender())) {
        mPopmenuTrackList->display(tracklistView->mapToGlobal(pos), tracklistView->selectedRows().size());
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

    mPlaylistView->editCurrentName();
}

void TPlaylistWindow::slotActionRemovePlaylist()
{
    if(mPlaylistView->selectedRows().size()>0 && verified())
        emit requestRemovePlaylist();

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

    if(path.isEmpty())
        return;

    QDir dir(path);
    if(!dir.isReadable())
        return;

    tryAddMusicFiles(dir.entryList(QDir::Files));
}

void TPlaylistWindow::slotOnActionRemoveSelectionsTriggered()
{
    if(!mMusiclistView)
        return;

    QList<int> selected = mMusiclistView->selectedRows();
    if(selected.size()>0 && verified())
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

    if(verified())
        emit requestRemoveAll();
}

void TPlaylistWindow::slotReparseMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    int row = mMusiclistView->currentRow();
    if(row != -1)
        emit requestReparseMusicItem(row);
}

void TPlaylistWindow::slotPlayMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    int row = mMusiclistView->currentRow();
    if(row != -1)
        emit requestPlayMusicItem(row);
}

void TPlaylistWindow::slotCopyMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    QList<int> rows = mMusiclistView->selectedRows();
    if(rows.size() > 0)
        emit requestCopyMusicItem(rows);
}

void TPlaylistWindow::slotCutMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    QList<int> rows = mMusiclistView->selectedRows();
    if(rows.size() > 0)
        emit requestCutMusicItem(rows);
}

void TPlaylistWindow::slotPasteMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    int row = mMusiclistView->currentRow();
    if(row < 0)
    {
        QAbstractItemModel *m = mMusiclistView->model();
        if(m)
            row = m->rowCount() - 1;
        else
            m = 0;
    }
    emit requestPasteMusicItem(row);
}

void TPlaylistWindow::slotDeleteMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    QList<int> rows = mMusiclistView->selectedRows();
    if(rows.size()>0 && verified())
        emit requestDeleteMusicItem(rows);
}

void TPlaylistWindow::slotRenameMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    mMusiclistView->editCurrentName();
}

void TPlaylistWindow::slotExplorerMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    int row = mMusiclistView->currentRow();
    if(row != -1)
        emit requestExplorerMusicItem(row);
}

void TPlaylistWindow::slotExportMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    int row = mMusiclistView->currentRow();
    if(row != -1)
        emit requestExportMusicItem(row);
}

void TPlaylistWindow::slotViewMusicItemTriggered()
{
    if(!mMusiclistView)
        return;

    int row = mMusiclistView->currentRow();
    if(row != -1)
        emit requestViewMusicItem(row);
}

void TPlaylistWindow::slotPlayTrackItemTriggered()
{
    if(!mTracklistView)
        return;

    int row = mTracklistView->currentRow();
    if(row != -1)
        emit requestPlayTrackItem(row);
}

void TPlaylistWindow::slotCopyTrackItemTriggered()
{
    if(!mTracklistView)
        return;

    QList<int> rows = mTracklistView->selectedRows();
    if(rows.size() > 0)
        emit requestCopyTrackItem(rows);
}

void TPlaylistWindow::slotChangeTrackItemTriggered()
{
    if(!mTracklistView)
        return;

    mTracklistView->editCurrent();
}

void TPlaylistWindow::slotExportTrackItemTriggered()
{
    if(!mTracklistView)
        return;

    QList<int> rows = mTracklistView->selectedRows();
    if(rows.size() > 0)
        emit requestExportTrackItem(rows);
}

void TPlaylistWindow::slotViewTrackItemTriggered()
{
    if(!mTracklistView)
        return;

    int row = mTracklistView->currentRow();
    if(row != -1)
        emit requestViewTrackItem(row);
}

void TPlaylistWindow::tryAddMusicFiles(QStringList files)
{
    if(!mMusiclistView)
        return;

    mMusiclistView->addFiles(files);
}

bool TPlaylistWindow::verified()
{
    return QMessageBox::question(this, tr("Question"), tr("Are you sure?"))==QMessageBox::Yes;
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

    if(mCentralWidget->updatePosition(size()))
    {
        mBtnClose->updatePos();
        mToolbar->updatePos();
    }

    update();
}
