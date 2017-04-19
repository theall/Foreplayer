#include "playlistcontroller.h"

#include <QUuid>
#include <QProcess>
#include <QMimeData>
#include <QClipboard>
#include <QSharedMemory>

void cpy2wchar(wchar_t *dest, QString source)
{
    if(dest)
    {
        wstring sourceW = source.toStdWString();
        wcscpy(dest, sourceW.c_str());
    }
}

TPlaylistController::TPlaylistController(QObject *parent) :
    TAbstractController(parent)
  , mPlaylistModel(new TPlaylistModel(this))
  , mMusiclistModel(new TMusiclistModel(this))
  , mTracklistModel(new TTrackListModel(this))
  , mMissionsModel(new TMissionsModel(this, &mExportMissions))
  , mPlaylistWindow(NULL)
  , mPlaylistWidget(NULL)
  , mPlaylistView(NULL)
  , mMusiclistView(NULL)
  , mTracklistView(NULL)
  , mExportDialog(NULL)
  , mExportMissionDialog(NULL)
  , mPropertyDialog(NULL)
  , mCurrentViewRow(-1)
  , mCurrentViewMusic(true)
{

}

TPlaylistController::~TPlaylistController()
{
    mExportMissionsLock.lock();
    for(QSharedMemory *m : mExportMissions) {
        m->detach();
        delete m;
    }
    mExportMissions.clear();
    mExportMissionsLock.unlock();
}

bool TPlaylistController::joint(TGuiManager *gui, TCore *core)
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

    mExportDialog = gui->exportDialog();
    mExportMissionDialog = gui->exportMissionDialog();
    mPropertyDialog = gui->propertyDialog();
    mExportMissionDialog->setModel(mMissionsModel);

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

    // Music list
    connect(mPlaylistWindow, SIGNAL(requestRemoveSelections(QList<int>)), this, SLOT(slotRequestRemoveSelections(QList<int>)));
    connect(mPlaylistWindow, SIGNAL(requestRemoveRedundant()), this, SLOT(slotRequestRemoveRedundant()));
    connect(mPlaylistWindow, SIGNAL(requestRemoveErrors()), this, SLOT(slotRequestRemoveErrors()));
    connect(mPlaylistWindow, SIGNAL(requestRemoveAll()), this, SLOT(slotRequestRemoveAll()));

    // Music item
    connect(mPlaylistWindow, SIGNAL(requestReparseMusicItem(int)), this, SLOT(slotRequestReparseMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestPlayMusicItem(int)), this, SLOT(slotRequestPlayMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestCopyMusicItem(QSet<int>)), this, SLOT(slotRequestCopyMusicItem(QSet<int>)));
    connect(mPlaylistWindow, SIGNAL(requestCutMusicItem(QSet<int>)), this, SLOT(slotRequestCutMusicItem(QSet<int>)));
    connect(mPlaylistWindow, SIGNAL(requestPasteMusicItem(int)), this, SLOT(slotRequestPasteMusicItem(int)));
    connect(mPlaylistWindow, SIGNAL(requestDeleteMusicItem(QSet<int>)), this, SLOT(slotRequestDeleteMusicItem(QSet<int>)));
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
    connect(mPlaylistWindow, SIGNAL(requestCopyTrackItem(QSet<int>)), this, SLOT(slotRequestCopyTrackItem(QSet<int>)));
    connect(mPlaylistWindow, SIGNAL(requestExportTrackItem(int)), this, SLOT(slotRequestExportTrackItem(int)));
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

    int playingPlaylistIndex = mCore->getPlayingIndex(IT_PL);
    slotPlaylistIndexChanged(playingPlaylistIndex);

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
    if(!mExportDialog || !mExportMissionDialog)
        return;

    QStringList indexList = mExportDialog->getIndexInfo();
    TExportMissions newMissions;
    for(QString index : indexList) {
        QString strId = QUuid::createUuid().toString();
        QSharedMemory *sharedMemory = new QSharedMemory(strId);
        if(!sharedMemory->create(sizeof(TExportParam)))
            continue;
        sharedMemory->attach();
        TExportParam exportParam;
        cpy2wchar(exportParam.fileName, mExportDialog->getMusicFileName());
        cpy2wchar(exportParam.indexName, index);
        cpy2wchar(exportParam.outputPath, mExportDialog->getOutputDir());
        strcpy(exportParam.format, mExportDialog->getFormat().toLocal8Bit().constData());
        exportParam.overwrite = true;
        exportParam.state = ES_READY;
        memcpy(sharedMemory->data(), &exportParam, sizeof(TExportParam));
        newMissions.append(sharedMemory);
    }
    mExportMissionsLock.lock();
    mMissionsModel->addMissions(newMissions);
    mExportMissionsLock.unlock();

    if(mExportMissions.size() > 0)
        startTimer(300);

    if(!mExportMissionDialog->isVisible())
        mExportMissionDialog->show();
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
    if(!mCore || !mTracklistModel)
        return;

    PlayListItem playlistItem = mCore->getPlaylistItem(mPlaylistModel->currentIndex());
    if(playlistItem)
    {
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
    mTracklistModel->setMusicItem(mCore->getMusicItem(mMusiclistModel->playlistItem(), pos));
}

void TPlaylistController::slotRequestRemoveSelections(QList<int> indexes)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeSelections(indexes);
}

void TPlaylistController::slotRequestRemoveRedundant()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeRedundant();
    mTracklistModel->setMusicItem(mCore->getMusicItem(mCore->getPlayingPlaylistItem(), mMusiclistModel->currentIndex()));
}

void TPlaylistController::slotRequestRemoveErrors()
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeErrors();
    mTracklistModel->setMusicItem(mCore->getMusicItem(mCore->getPlayingPlaylistItem(), mMusiclistModel->currentIndex()));
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

void TPlaylistController::slotRequestCopyMusicItem(QSet<int> rows)
{
    if(!mCore || !mMusiclistModel)
        return;

    PlayListItem playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        QClipboard *clipBoard = qApp->clipboard();
        QMimeData *mimeData = new QMimeData;
        mimeData->setData(MIME_TYPE_MUSIC_ITEM, mCore->musicItemsToString(playlistItem, rows).toLocal8Bit());
        clipBoard->setMimeData(mimeData);
    }
}

void TPlaylistController::slotRequestCutMusicItem(QSet<int> rows)
{
    if(!mMusiclistModel)
        return;

    PlayListItem playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        QClipboard *clipBoard = qApp->clipboard();
        for(int row : rows) {
            MusicItem musicItem = mCore->getMusicItem(playlistItem, row);
            if(musicItem)
            {
                QMimeData *mimeData = new QMimeData;
                //mimeData->setData(MIME_TYPE_MUSIC_ITEM, mCore->getMusicItemtoJson());
                clipBoard->setMimeData(mimeData);
            }
        }
    }
}

void TPlaylistController::slotRequestPasteMusicItem(int pos)
{
    if(!mMusiclistModel)
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
    mMusiclistModel->insertItems(pos, musicItems);
}

void TPlaylistController::slotRequestDeleteMusicItem(QSet<int> rows)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeSelections(rows.toList());
}

void TPlaylistController::slotRequestExplorerMusicItem(int row)
{
    if(!mMusiclistModel)
        return;

    MusicItem musicItem = mCore->getMusicItem(mMusiclistModel->playlistItem(), row);
    if(musicItem)
    {
        QString fileName = mCore->getMusicItemFileName(musicItem);
#ifdef Q_OS_WIN32
        QProcess::startDetached("explorer /select,"+QDir::toNativeSeparators(fileName));
#elif Q_OS_UNIX
#elif Q_OS_MACX
#endif
    }
}

void TPlaylistController::slotRequestExportMusicItem(int row)
{
    if(!mMusiclistModel || !mExportDialog || !mExportMissionDialog)
        return;

    MusicItem musicItem = mCore->getMusicItem(mMusiclistModel->playlistItem(), row);
    if(musicItem)
    {
        QStringList indexList;
        for(TrackItem trackItem : mCore->getTrackItems(musicItem)) {
            indexList.append(mCore->getTrackItemIndexName(trackItem));
        }
        QString musicItemFileName = mCore->getMusicItemFileName(musicItem);
        mExportDialog->setMusicFile(musicItemFileName);
        mExportDialog->setMaxDuration(0);
        mExportDialog->setIndexInfo(indexList);
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

void TPlaylistController::slotRequestCopyTrackItem(QSet<int> rows)
{
    if(!mTracklistModel)
        return;

    MusicItem musicItem = mTracklistModel->musicItem();
    if(musicItem)
    {
        QClipboard *clipBoard = qApp->clipboard();
        QMimeData *mimeData = new QMimeData;
        mimeData->setData(MIME_TYPE_TRACK_ITEM, mCore->getTrackItemsAsString(musicItem, rows).toLocal8Bit());
        clipBoard->setMimeData(mimeData);
    }
}

void TPlaylistController::slotRequestExportTrackItem(int row)
{
    if(!mTracklistModel || !mExportDialog || !mExportMissionDialog)
        return;

    MusicItem musicItem = mTracklistModel->musicItem();
    if(musicItem)
    {
        TrackItem trackItem = mCore->getTrackItem(musicItem, row);
        if(trackItem)
        {
            QString musicFileName = mCore->getMusicItemFileName(musicItem);
            mExportDialog->setMusicFile(musicFileName);
            mExportDialog->setMaxDuration(mCore->getTrackItemDuration(trackItem));
            mExportDialog->setIndexInfo(mCore->getTrackItemIndexName(trackItem));
            if(mExportDialog->getOutputDir().isEmpty())
                mExportDialog->setOutputPath(QFileInfo(musicFileName).absolutePath());
            mExportDialog->exec();
        }
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
    }
}

void TPlaylistController::slotTimerEvent()
{
    if(mMissionsModel && mExportMissionDialog && mExportMissionDialog->isVisible())
    {
        mExportMissionsLock.lock();
        int runningCount = 0;
        int completeCount = 0;
        for(QSharedMemory *m : mExportMissions) {
            TExportParam *exportParam = (TExportParam*)m->data();
            if(exportParam->state == ES_RUN)
                runningCount++;
            else if(exportParam->state == ES_COMPLETE)
                completeCount++;
        }

        // Maximize 3 processes
        int newSpawn = 3-runningCount;
        if(newSpawn > 0)
        {
            QString commandLine = "exportor ";
#ifndef QT_DEBUG
            commandLine.prepend("noconsole ");
#endif
            for(QSharedMemory *m : mExportMissions) {
                TExportParam *exportParam = (TExportParam*)m->data();
                if(exportParam->state == ES_READY)
                {
                    QProcess::startDetached(commandLine+m->key());
                    qDebug() << commandLine+m->key();
                    exportParam->state = ES_RUN;
                    newSpawn--;
                    if(newSpawn <= 0)
                        break;
                }
            }
        } else {
            if(completeCount >= mExportMissions.size())
                stopTimer();
        }
        mExportMissionsLock.unlock();
        mMissionsModel->layoutChanged();
    }
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
