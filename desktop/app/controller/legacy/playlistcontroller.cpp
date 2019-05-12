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
#include "playlistcontroller.h"

#include "utils.h"

#include <QTimer>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

TPlaylistController::TPlaylistController(QObject *parent) :
    TAbstractController(parent)
  , mPlaylistModel(new TPlaylistModel(this))
  , mMusiclistModel(new TMusiclistModel(this))
  , mTracklistModel(new TTrackListModel(this))
  , mPlaylistWindow(NULL)
  , mPlaylistWidget(NULL)
  , mPlaylistView(NULL)
  , mMusiclistView(NULL)
  , mTracklistView(NULL)
  , mExportDialog(NULL)
  , mPropertyDialog(NULL)
  , mCurrentViewRow(-1)
  , mCurrentViewMusic(true)
{

}

TPlaylistController::~TPlaylistController()
{

}

bool TPlaylistController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    TAbstractController::joint(gui, core);

    mPlaylistWindow = gui->playlistWindow();
    Q_ASSERT(mPlaylistWindow);

    mPlaylistWidget = mPlaylistWindow->playlistWidget();
    Q_ASSERT(mPlaylistWidget);

    mPlaylistView = mPlaylistWidget->playlistView();
    mMusiclistView = mPlaylistWidget->musiclistView();
    mTracklistView = mPlaylistWidget->tracklistView();

    mExportDialog = gui->exportDialog();
    mPropertyDialog = gui->propertyDialog();

    slotSkinChanged();

    mPlaylistModel->setPlayerCore(core);
    mMusiclistModel->setPlayerCore(core);
    mTracklistModel->setPlayerCore(core);

    mPlaylistView->setModel(mPlaylistModel);
    mMusiclistView->setModel(mMusiclistModel);
    mTracklistView->setModel(mTracklistModel);

    connect(gui, SIGNAL(skinChanged()), this, SLOT(slotSkinChanged()));

    connect(mPlaylistView, SIGNAL(onCurrentRowChanged(int)), this, SLOT(slotPlaylistIndexChanged(int)));
    connect(mMusiclistView, SIGNAL(onCurrentRowChanged(int)), this, SLOT(slotMusiclistIndexChanged(int)));
    connect(mTracklistView, SIGNAL(onCurrentRowChanged(int)), this, SLOT(slotTracklistIndexChanged(int)));

    connect(mPlaylistView, SIGNAL(onDoubleClickItem(int)), this, SLOT(slotPlaylistItemSelected(int)));
    connect(mMusiclistView, SIGNAL(onDoubleClickItem(int)), this, SLOT(slotMusiclistItemSelected(int)));
    connect(mTracklistView, SIGNAL(onDoubleClickItem(int)), this, SLOT(slotTracklistItemSelected(int)));

    // Play list
    connect(mPlaylistWindow, SIGNAL(requestAddNewPlaylist()), this, SLOT(slotRequestAddNewPlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestRemovePlaylist()), this, SLOT(slotRequestRemovePlaylist()));
    connect(mPlaylistWindow, SIGNAL(requestSortPlaylists()), this, SLOT(slotRequestSortPlaylists()));
    connect(mPlaylistWindow, SIGNAL(requestSendTo()), this, SLOT(slotRequestSendTo()));
    connect(mPlaylistWindow, SIGNAL(requestSupportSuffixList(QList<QPair<QString,QString> >&)), this, SLOT(slotRequestSupportSuffixList(QList<QPair<QString,QString> >&)));

    // Music list
    connect(mPlaylistWindow, SIGNAL(requestRemoveSelections(QList<int>)), this, SLOT(slotRequestDeleteMusicItem(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestRemoveRedundant()), this, SLOT(slotRequestRemoveRedundant()));
    connect(mPlaylistWindow, SIGNAL(requestRemoveErrors()), this, SLOT(slotRequestRemoveErrors()));
    connect(mPlaylistWindow, SIGNAL(requestRemoveAll()), this, SLOT(slotRequestRemoveAll()));

    // Music item
    connect(mPlaylistWindow, SIGNAL(requestReparseMusicItem(int)), this, SLOT(slotRequestReparseMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestPlayMusicItem(int)), this, SLOT(slotRequestPlayMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestCopyMusicItem(QList<int>)), this, SLOT(slotRequestCopyMusicItem(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestCutMusicItem(QList<int>)), this, SLOT(slotRequestCutMusicItem(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestPasteMusicItem(int)), this, SLOT(slotRequestPasteMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestDeleteMusicItem(QList<int>)), this, SLOT(slotRequestDeleteMusicItem(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestExplorerMusicItem(int)), this, SLOT(slotRequestExplorerMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestExportMusicItem(int)), this, SLOT(slotRequestExportMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestViewMusicItem(int)), this, SLOT(slotRequestViewMusicItem(int)));

    // Sort music list
    connect(mPlaylistWindow, SIGNAL(requestSortMusiclistAsName()), this, SLOT(slotRequestSortMusiclistAsName()));
    connect(mPlaylistWindow, SIGNAL(requestSortMusiclistAsArtist()), this, SLOT(slotRequestSortMusiclistAsArtist()));
    connect(mPlaylistWindow, SIGNAL(requestSortMusiclistAsAlbum()), this, SLOT(slotRequestSortMusiclistAsAlbum()));
    connect(mPlaylistWindow, SIGNAL(requestSortMusiclistAsDuration()), this, SLOT(slotRequestSortMusiclistAsDuration()));
    connect(mPlaylistWindow, SIGNAL(requestSortMusiclistRandom()), this, SLOT(slotRequestSortMusiclistRandom()));
    connect(mPlaylistWindow, SIGNAL(requestSortMusiclistReverse()), this, SLOT(slotRequestSortMusiclistReverse()));

    // Track item
    connect(mPlaylistWindow, SIGNAL(requestPlayTrackItem(int)), this, SLOT(slotRequestPlayTrackItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestCopyTrackItem(QList<int>)), this, SLOT(slotRequestCopyTrackItem(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestExportTrackItem(QList<int>)), this, SLOT(slotRequestExportTrackItem(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestViewTrackItem(int)), this, SLOT(slotRequestViewTrackItem(int)));

    // list view
    connect(mPlaylistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMovePlaylists(QList<int>, int, QList<int>&)));
    connect(mMusiclistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMoveMusics(QList<int>, int, QList<int>&)));
    connect(mMusiclistView, SIGNAL(requestAddFiles(QStringList,int,QList<int>&)), this, SLOT(slotRequestAddMusicFiles(QStringList, int, QList<int>&)));
    connect(mTracklistView, SIGNAL(requestMoveItems(QList<int>,int,QList<int>&)), this, SLOT(slotRequestMoveTracks(QList<int>, int, QList<int>&)));

    // Export dialog
    connect(mExportDialog, SIGNAL(onAddClicked()), this, SLOT(slotAddExportMission()));

    // Property dialog
    connect(mPropertyDialog, SIGNAL(onPreviousClicked()), this, SLOT(slotRequestPreviousMusicProperty()));
    connect(mPropertyDialog, SIGNAL(onNextClicked()), this, SLOT(slotRequestNextMusicProperty()));

    // Misc
    connect(mTracklistModel, SIGNAL(requestAdjustColumnWidth()), this, SLOT(slotRequestAdjustColumnWidth()));

    int playingPlaylistIndex = -1;
    int playingMusicIndex = -1;
    int playingTrackIndex = -1;
    mCore->getPlayingIndex(&playingPlaylistIndex, &playingMusicIndex, &playingTrackIndex);
    mPlaylistModel->setPlayingIndex(playingPlaylistIndex);
    slotPlaylistIndexChanged(playingPlaylistIndex);

    locateIndex(playingPlaylistIndex, playingMusicIndex, playingTrackIndex);

    return true;
}

void TPlaylistController::slotSkinChanged()
{
    if(mPlaylistWidget)
        TAbstractModel::setColorParameters(
            mPlaylistWidget->textFont(),
            mPlaylistWidget->colorText(),
            mPlaylistWidget->colorHilight(),
            mPlaylistWidget->colorNumber(),
            mPlaylistWidget->colorDuration(),
            mPlaylistWidget->colorSelect(),
            mPlaylistWidget->colorBkgnd(),
            mPlaylistWidget->colorBkgnd2());
}

void TPlaylistController::slotRequestCurrentIndex(int *pIndex, int *mIndex, int *tIndex)
{
    if(!mMusiclistModel || !mPlaylistModel || !mTracklistModel)
        return;

    *pIndex = mPlaylistModel->currentIndex();
    *mIndex = mMusiclistModel->currentIndex();
    *tIndex = mTracklistModel->currentIndex();
}

// Send from playercontroller
void TPlaylistController::slotRequestFixDuration(int microSeconds)
{
    if(!mTracklistModel || !mMusiclistModel)
        return;

    MusicItem musicItem = mTracklistModel->musicItem();
    TrackItem trackItem = mCore->getPlayingTrackItem();
    if(mCore->getTrackItemIndex(musicItem, trackItem) < 0)
        return;

    // Change duration
    int trackItemDuration = mCore->getTrackItemDuration(trackItem);
    if(microSeconds != trackItemDuration)
    {
        mCore->setTrackItemDuration(musicItem, trackItem, microSeconds);
        mTracklistModel->update();
        mMusiclistModel->update();
    }
}

void TPlaylistController::slotAddExportMission()
{
    if(!mExportDialog)
        return;

    QList<QPair<QString, QString>> indexInfo = mExportDialog->getIndexInfo();
    int size = indexInfo.size();
    if(size > 0)
    {
        TExportParam *params = new TExportParam[size];
        int i = 0;
        bool autoNumber = mExportDialog->autoNumber();
        bool overWrite = mExportDialog->overWrite();
        QString musicFileName = mExportDialog->getMusicFileName();
        QString outputPath = mExportDialog->getOutputDir();
        QString format = mExportDialog->getFormat();
        for(QPair<QString, QString> indexPair : indexInfo) {
            TExportParam *exportParam = &params[i++];
            Utils::cpy2wchar(exportParam->fileName, musicFileName);
            Utils::cpy2wchar(exportParam->indexName, indexPair.first);
            Utils::cpy2wchar(exportParam->title, indexPair.second);
            Utils::cpy2wchar(exportParam->outputPath, outputPath);
            strcpy(exportParam->format, format.toLocal8Bit().constData());
            exportParam->overwrite = overWrite;
            if(autoNumber)
                exportParam->number = i;
            exportParam->state = ES_NULL;
        }
        emit requestExport(params, size);
        delete params;
    }
}

void TPlaylistController::slotRequestNextMusicProperty()
{
    if(!mMusiclistModel || !mTracklistModel)
        return;

    int trackCount = 0;
    if(mCurrentViewMusic)
    {
        PlayListItem playlistItem = mMusiclistModel->playlistItem();
        if(playlistItem)
            trackCount = mCore->getMusicItemCount(playlistItem);
    } else {
        MusicItem musicItem = mTracklistModel->musicItem();
        if(musicItem)
            trackCount = mCore->getTrackItemCount(musicItem);
    }
    if(mCurrentViewRow<trackCount-1 && trackCount>0)
    {
        mCurrentViewRow++;
        fillPropertyDialog();
    }
}

void TPlaylistController::slotRequestPreviousMusicProperty()
{
    if(!mMusiclistModel || !mTracklistModel)
        return;

    int trackCount = 0;
    if(mCurrentViewMusic)
    {
        PlayListItem playlistItem = mMusiclistModel->playlistItem();
        if(playlistItem)
            trackCount = mCore->getMusicItemCount(playlistItem);
    } else {
        MusicItem musicItem = mTracklistModel->musicItem();
        if(musicItem)
            trackCount = mCore->getTrackItemCount(musicItem);
    }
    if(mCurrentViewRow>0 && trackCount>0)
    {
        mCurrentViewRow--;
        fillPropertyDialog();
    }
}

void TPlaylistController::slotRequestAdjustColumnWidth()
{
    if(mTracklistView)
        mTracklistView->updateColumnsWidth();

    if(mMusiclistView)
    {
        mMusiclistView->updateColumnsWidth();
        mMusiclistView->update();
    }
}

void TPlaylistController::slotPlaylistIndexChanged(int index)
{
    if(!mMusiclistModel || !mPlaylistModel || !mCore)
        return;

    mPlaylistModel->setCurrentIndex(index);

    PlayListItem item = mCore->getPlaylistItem(index);
    mMusiclistModel->setPlayListItem(item);

    int musicIndex = 0;

    // Check if is playing index as the playing index in models will reset after every set item
    if(index == mCore->getPlayingIndex(IT_PL))
    {
        // Is current playing playlist
        musicIndex = mCore->getPlayingIndex(IT_ML);
        mMusiclistModel->setPlayingIndex(musicIndex);
    }
    mMusiclistModel->setCurrentIndex(musicIndex);

    // Change track list model while playlist index changed
    slotMusiclistIndexChanged(musicIndex);
}

void TPlaylistController::slotMusiclistIndexChanged(int index)
{
    if(!mCore || !mMusiclistModel || !mTracklistModel)
        return;

    PlayListItem playlistItem = mCore->getPlaylistItem(mPlaylistModel->currentIndex());
    if(playlistItem)
    {
        mMusiclistModel->setCurrentIndex(index);

        MusicItem musicItem = mCore->getMusicItem(playlistItem, index);
        mTracklistModel->setMusicItem(musicItem);

        int trackIndex = 0;

        // Check if this music item is playing
        if(mCore->getPlayingMusicItem()==musicItem)
        {
            trackIndex = mCore->getPlayingIndex(IT_TL);
            mTracklistModel->setPlayingIndex(trackIndex);
        }

        mTracklistModel->setCurrentIndex(trackIndex);
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

    int musicItemIndex = mCore->getMusicItemIndex(mMusiclistModel->playlistItem(), mTracklistModel->musicItem());
    if(musicItemIndex > -1)
        // Play current track
        emit requestPlay(
                    mPlaylistModel->currentIndex(),
                    musicItemIndex,
                    mTracklistModel->currentIndex());
}

void TPlaylistController::slotRequestAddNewPlaylist()
{
    if(!mPlaylistModel || !mPlaylistView)
        return;

    int newIndex = mPlaylistModel->add(tr("New playlist"));
    mPlaylistView->selectRow(newIndex);
    slotPlaylistIndexChanged(newIndex);
}

void TPlaylistController::slotRequestRemovePlaylist()
{
    if(!mPlaylistModel || !mPlaylistView)
        return;

    int currentIndex = mPlaylistModel->currentIndex();

    mPlaylistModel->remove(currentIndex);

    // Reget current index after item removed
    currentIndex = mPlaylistModel->currentIndex();
    if(currentIndex < 0)
    {
        QList<int> selected = mPlaylistView->selectedRows();
        if(selected.size() > 0)
            currentIndex = selected.at(0);
    }

    if(currentIndex > -1)
        slotPlaylistIndexChanged(currentIndex);
}

void TPlaylistController::slotRequestSortPlaylists()
{
    if(!mPlaylistModel)
        return;

    mPlaylistModel->sortItems();
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
    if(newIndexes.size() > 0)
        mTracklistModel->setMusicItem(mCore->getMusicItem(mMusiclistModel->playlistItem(), newIndexes.at(0)));
}

void TPlaylistController::slotRequestSupportSuffixList(QList<QPair<QString, QString> > &suffixList)
{
    if(!mCore)
        return;

    for(PluginHandle plugin : mCore->getPluginHandles())
        suffixList += mCore->getPluginSuffixDescription(plugin);
}

void TPlaylistController::slotRequestDeleteMusicItem(QList<int> indexes)
{
    if(!mMusiclistModel || !mMusiclistView)
        return;

    int oldIndex = mMusiclistModel->currentIndex();

    mMusiclistModel->removeSelections(indexes);

    restoreMusicListSelection(oldIndex);
}

void TPlaylistController::slotRequestRemoveRedundant()
{
    if(!mMusiclistModel)
        return;

    int oldIndex = mMusiclistModel->currentIndex();

    mMusiclistModel->removeRedundant();
    mTracklistModel->setMusicItem(mCore->getMusicItem(mCore->getPlayingPlaylistItem(), mMusiclistModel->currentIndex()));

    restoreMusicListSelection(oldIndex);
}

void TPlaylistController::slotRequestRemoveErrors()
{
    if(!mMusiclistModel)
        return;

    int oldIndex = mMusiclistModel->currentIndex();

    mMusiclistModel->removeErrors();
    mTracklistModel->setMusicItem(mCore->getMusicItem(mCore->getPlayingPlaylistItem(), mMusiclistModel->currentIndex()));

    restoreMusicListSelection(oldIndex);
}

void TPlaylistController::slotRequestRemoveAll()
{
    if(!mMusiclistModel || !mTracklistModel)
        return;

    mTracklistModel->setMusicItem(NULL);
    mMusiclistModel->removeAll();
}

void TPlaylistController::slotRequestReparseMusicItem(int row)
{
    if(!mMusiclistModel || !mCore)
        return;

    PlayListItem playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        MusicItem musicItem = mCore->getMusicItem(playlistItem, row);
        if(musicItem)
        {
            MusicItem newItem = mCore->parse(mCore->getMusicItemFileName(musicItem));
            int index = mCore->getMusicItemIndex(playlistItem, musicItem);
            mCore->updateMusicItem(playlistItem, index, newItem);
            mTracklistModel->setMusicItem(newItem);
        }
    }
}

void TPlaylistController::slotRequestPlayMusicItem(int row)
{
    slotMusiclistItemSelected(row);
}

void TPlaylistController::slotRequestCopyMusicItem(QList<int> rows)
{
    if(!mCore || !mMusiclistModel)
        return;

    PlayListItem playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        MusicItems musicItems;
        for(int row : rows)
            musicItems.append(mCore->getMusicItem(playlistItem, row));
        QClipboard *clipBoard = qApp->clipboard();
        QMimeData *mimeData = new QMimeData;
        mimeData->setData(MIME_TYPE_MUSIC_ITEM, mCore->musicItemToString(musicItems).toLocal8Bit());
        clipBoard->setMimeData(mimeData);
    }
}

void TPlaylistController::slotRequestCutMusicItem(QList<int> rows)
{
    if(!mMusiclistModel || !mMusiclistView || !mTracklistModel)
        return;

    slotRequestCopyMusicItem(rows);
    mMusiclistModel->removeSelections(rows);

    // Check whether current music item is removed
    PlayListItem playlistItem = mMusiclistModel->playlistItem();
    MusicItem musicItem = mTracklistModel->musicItem();
    if(playlistItem && musicItem)
    {
        if(mCore->getMusicItemIndex(playlistItem, musicItem) < 0)
            mTracklistModel->setMusicItem(NULL);
    }
}

void TPlaylistController::slotRequestPasteMusicItem(int pos)
{
    if(!mMusiclistModel || !mMusiclistView)
        return;

    MusicItems musicItems;
    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    if(mimeData->hasFormat(MIME_TYPE_MUSIC_ITEM)) {
        musicItems = mCore->stringToMusicItems(mimeData->data(MIME_TYPE_MUSIC_ITEM));
    } else if (mimeData->hasFormat(MIME_TYPE_TRACK_ITEM)) {
//        QJsonArray trackItemsArray = QJsonDocument::fromJson(mimeData->data(MIME_TYPE_TRACK_ITEM)).array();
//        for(int i=0;i<trackItemsArray.size();i++)
//        {
//            MusicItem musicItem = new MusicItem;
//            TTrackItem trackItem = new TTrackItem;
//            trackItem->fromJson(trackItemsArray.at(i).toObject());
//            mCore->getMusicItemfromTrackItem(trackItem);
//            mCore->getMusicItemaddTrackItem(trackItem);
//            musicItems.append(musicItem);
//        }
    }

    if(musicItems.size() > 0)
        mMusiclistView->selectRows(mMusiclistModel->insertItems(pos, musicItems));
}

void TPlaylistController::slotRequestExplorerMusicItem(int row)
{
    if(!mMusiclistModel)
        return;

    MusicItem musicItem = mCore->getMusicItem(mMusiclistModel->playlistItem(), row);
    if(musicItem)
    {
        Utils::exploreFile(mCore->getMusicItemFileName(musicItem));
    }
}

void TPlaylistController::slotRequestExportMusicItem(int row)
{
    if(!mMusiclistModel || !mExportDialog)
        return;

    MusicItem musicItem = mCore->getMusicItem(mMusiclistModel->playlistItem(), row);
    if(musicItem)
    {
        QList<QPair<QString, QString>> indexInfo;
        for(TrackItem trackItem : mCore->getTrackItems(musicItem)) {
            indexInfo.append(qMakePair(mCore->getTrackItemIndexName(trackItem), mCore->getTrackItemName(trackItem)));
        }
        QString musicItemFileName = mCore->getMusicItemFileName(musicItem);
        mExportDialog->setMusicTitle(mCore->getMusicItemDisplayName(musicItem));
        mExportDialog->setMusicFile(musicItemFileName);
        mExportDialog->setMaxDuration(0);
        mExportDialog->setIndexInfo(indexInfo);
        if(mExportDialog->getOutputDir().isEmpty())
            mExportDialog->setOutputPath(QFileInfo(musicItemFileName).absolutePath());
        mExportDialog->exec();
    }
}

void TPlaylistController::slotRequestViewMusicItem(int row)
{
    if(!mMusiclistModel || !mPropertyDialog)
        return;

    mCurrentViewRow = row;
    mCurrentViewMusic = true;

    fillPropertyDialog();
    mPropertyDialog->exec();
}

void TPlaylistController::slotRequestSortMusiclistAsName()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->sortItems(SM_TITLE_ASC);
}

void TPlaylistController::slotRequestSortMusiclistAsArtist()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->sortItems(SM_ARTIST_ASC);
}

void TPlaylistController::slotRequestSortMusiclistAsAlbum()
{
    if(!mMusiclistModel)
        return;
    mMusiclistModel->sortItems(SM_SYSTEM_ASC);
}

void TPlaylistController::slotRequestSortMusiclistAsDuration()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->sortItems(SM_DURATION_ASC);
}

void TPlaylistController::slotRequestSortMusiclistRandom()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->sortItems(SM_RANDOM);
}

void TPlaylistController::slotRequestSortMusiclistReverse()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->sortItems(SM_REVERSE);
}

void TPlaylistController::slotRequestPlayTrackItem(int row)
{
    if(row >= 0)
        slotTracklistItemSelected(row);
}

void TPlaylistController::slotRequestCopyTrackItem(QList<int> rows)
{
    if(!mTracklistModel)
        return;

    MusicItem musicItem = mTracklistModel->musicItem();
    if(musicItem)
    {
        TrackItems trackItems;
        for(int row : rows)
            trackItems.append(mCore->getTrackItem(musicItem, row));
        QClipboard *clipBoard = qApp->clipboard();
        QMimeData *mimeData = new QMimeData;
        mimeData->setData(MIME_TYPE_TRACK_ITEM, mCore->trackItemToString(trackItems).toLocal8Bit());
        clipBoard->setMimeData(mimeData);
    }
}

void TPlaylistController::slotRequestExportTrackItem(QList<int> rows)
{
    if(!mCore || !mTracklistModel || !mExportDialog)
        return;

    MusicItem musicItem = mTracklistModel->musicItem();
    if(musicItem)
    {
        QList<QPair<QString, QString>> indexInfo;
        for(int row : rows) {
            TrackItem trackItem = mCore->getTrackItem(musicItem, row);
            if(!trackItem)
                continue;
            indexInfo.append(qMakePair(mCore->getTrackItemIndexName(trackItem), mCore->getTrackItemName(trackItem)));
        }
        QString musicItemFileName = mCore->getMusicItemFileName(musicItem);
        mExportDialog->setMusicTitle(mCore->getMusicItemDisplayName(musicItem));
        mExportDialog->setMusicFile(musicItemFileName);
        mExportDialog->setIndexInfo(indexInfo);
        if(mExportDialog->getOutputDir().isEmpty())
            mExportDialog->setOutputPath(QFileInfo(musicItemFileName).absolutePath());
        mExportDialog->exec();
    }
}

void TPlaylistController::slotRequestViewTrackItem(int row)
{
    if(!mTracklistModel || !mPropertyDialog)
        return;

    mCurrentViewRow = row;
    mCurrentViewMusic = false;

    fillPropertyDialog();
    mPropertyDialog->exec();
}

void TPlaylistController::slotRequestUpdateModelsPlayingIndex(int pi, int mi, int ti)
{
//    mPlaylistModel->setPlayingIndex(pi);
//    slotPlaylistIndexChanged(pi);
//    locateIndex(pi, mi, ti);
//    return;

    if(mPlaylistModel && mMusiclistModel && mTracklistModel && mCore)
    {
        mPlaylistModel->setPlayingIndex(pi);

        if(mCore->getPlayingPlaylistItem() == mMusiclistModel->playlistItem())
        {
            mMusiclistModel->setPlayingIndex(mi);
            if(mTracklistModel->musicItem() == mCore->getPlayingMusicItem())
                mTracklistModel->setPlayingIndex(ti);
            else
                mTracklistModel->setPlayingIndex(-1);
        } else {
            mMusiclistModel->setPlayingIndex(-1);
            mTracklistModel->setPlayingIndex(-1);
        }
        locateIndex(pi, mi, ti);
    }
}

void TPlaylistController::slotTimerEvent()
{
}

void TPlaylistController::fillPropertyDialog()
{
    if(!mPropertyDialog || !mMusiclistModel || !mTracklistModel)
        return;

    if(mCurrentViewMusic)
    {
        PlayListItem playlistItem = mMusiclistModel->playlistItem();
        if(playlistItem)
        {
            int musicCount = mCore->getMusicItemCount(playlistItem);
            MusicItem musicItem = mCore->getMusicItem(playlistItem, mCurrentViewRow);
            if(musicItem)
            {
                mPropertyDialog->setMusicFile(mCore->getMusicItemFileName(musicItem));
                mPropertyDialog->setTitle(mCore->getMusicItemDisplayName(musicItem));
                mPropertyDialog->setAuthor(mCore->getMusicItemArtist(musicItem));
                mPropertyDialog->setYear(mCore->getMusicItemYear(musicItem));
                mPropertyDialog->setSystem(mCore->getMusicItemType(musicItem));
                mPropertyDialog->setAddionalInfo(mCore->getMusicItemAdditionalInfo(musicItem));
                mPropertyDialog->setIndex(mCurrentViewRow+1, musicCount);
            }
        }
    } else {
        MusicItem musicItem = mTracklistModel->musicItem();
        if(musicItem)
        {
            int trackCount = mCore->getTrackItemCount(musicItem);
            TrackItem trackItem = mCore->getTrackItem(musicItem, mCurrentViewRow);
            if(trackItem)
            {
                mPropertyDialog->setMusicFile(mCore->getMusicItemFileName(musicItem));
                mPropertyDialog->setTitle(mCore->getTrackItemName(trackItem));
                mPropertyDialog->setAuthor(mCore->getTrackItemArtist(trackItem));
                mPropertyDialog->setYear(mCore->getTrackItemYear(trackItem));
                mPropertyDialog->setSystem(mCore->getTrackItemType(trackItem));
                mPropertyDialog->setAddionalInfo(mCore->getTrackItemAdditionalInfo(trackItem));
                mPropertyDialog->setIndex(mCurrentViewRow+1, trackCount);
            }
        }
    }
}

void TPlaylistController::restoreMusicListSelection(int oldIndex)
{
    int currentIndex = mMusiclistModel->currentIndex();
    int rowCount = mMusiclistModel->rowCount(QModelIndex()) - 1;

    // Reget current index after item removed
    if(currentIndex < 0)
    {
        currentIndex = oldIndex;
    }
    if(currentIndex >= rowCount) {
        currentIndex = rowCount - 1;
    }

    if(currentIndex > -1)
    {
        slotMusiclistIndexChanged(currentIndex);
        mMusiclistView->selectRow(currentIndex);
    }
}

void TPlaylistController::locateIndex(int pi, int mi, int ti)
{
    if(mPlaylistView)
    {
        //mPlaylistView->selectRow(pi);
        mPlaylistView->scrollTo(mPlaylistModel->index(pi, 0));
    }
    if(mMusiclistView)
    {
        //mMusiclistView->selectRow(mi);
        mMusiclistView->scrollTo(mMusiclistModel->index(mi, 1));
    }
    if(mTracklistView)
    {
        //mTracklistView->selectRow(ti);
        mTracklistView->scrollTo(mTracklistModel->index(ti, 1));
    }
}
