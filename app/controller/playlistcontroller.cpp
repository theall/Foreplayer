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
  , mPlaylistCore(NULL)
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
    foreach (QSharedMemory *m, mExportMissions) {
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

    mPlaylistCore = core->playlist();
    mPlaylistModel->setPlaylistCore(mPlaylistCore);
    mMusiclistModel->setPlaylistCore(mPlaylistCore);
    mTracklistModel->setPlaylistCore(mPlaylistCore);

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

    int playingPlaylistIndex = mPlaylistCore->playingPlaylistIndex();
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

    TMusicItem *musicItem = mTracklistModel->musicItem();
    TTrackItem *trackItem =  mPlaylistCore->currentTrackItem();
    if(!musicItem || !trackItem || musicItem->trackItems()->indexOf(trackItem)<0)
        return;

    // Change duration
    if(microSeconds != trackItem->duration)
    {
        int diff = microSeconds - trackItem->duration;
        trackItem->duration = microSeconds;
        musicItem->setDuration(musicItem->duration()+diff);
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
    foreach (QString index, indexList) {
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
        TPlaylistItem *playlistItem = mMusiclistModel->playlistItem();
        if(playlistItem)
            trackCount = playlistItem->size();
    } else {
        TMusicItem *musicItem = mTracklistModel->musicItem();
        if(musicItem)
            trackCount = musicItem->size();
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
        TPlaylistItem *playlistItem = mMusiclistModel->playlistItem();
        if(playlistItem)
            trackCount = playlistItem->size();
    } else {
        TMusicItem *musicItem = mTracklistModel->musicItem();
        if(musicItem)
            trackCount = musicItem->size();
    }
    if(mCurrentViewRow>0 && trackCount>0)
    {
        mCurrentViewRow--;
        fillPropertyDialog();
    }
}

void TPlaylistController::slotPlaylistIndexChanged(int index)
{
    if(!mMusiclistModel || !mPlaylistModel || !mPlaylistCore)
        return;

    mPlaylistModel->setCurrentIndex(index);

    TPlaylistItem *item = mPlaylistCore->playlistItem(index);
    mMusiclistModel->setPlayListItem(item);

    int musicIndex = 0;

    // Check if is playing index as the playing index in models will reset after every set item
    if(index == mPlaylistCore->playingPlaylistIndex())
    {
        // Is current playing playlist
        musicIndex = mPlaylistCore->playingMusicIndex();
        mMusiclistModel->setPlayingIndex(musicIndex);
    }
    mMusiclistModel->setCurrentIndex(musicIndex);

    // Change track list model while playlist index changed
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

        int trackIndex = 0;

        // Check if this music item is playing
        if(mPlaylistCore->currentMusicItem()==musicItem)
        {
            trackIndex = mPlaylistCore->playingTrackIndex();
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

    int musicItemIndex = mMusiclistModel->playlistItem()->musicItemIndex(mTracklistModel->musicItem());
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
    mTracklistModel->setMusicItem(mMusiclistModel->playlistItem()->musicItem(pos));
}

void TPlaylistController::slotRequestRemoveSelections(QList<int> indexes)
{
    if(!mMusiclistModel)
        return;

    mMusiclistModel->removeSelections(indexes);
    //mTracklistModel->setMusicItem(mMusiclistModel->playlistItem()->musicItem(pos));
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

void TPlaylistController::slotRequestReparseMusicItem(int row)
{
    if(!mMusiclistModel || !mPlaylistCore)
        return;

    TPlaylistItem *playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        TMusicItem *musicItem = playlistItem->musicItem(row);
        if(musicItem)
        {
            TMusicItem *newItem = mPlaylistCore->parse(musicItem->fileName());
            int index = playlistItem->indexOf(musicItem);
            playlistItem->update(index, newItem);
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
    if(!mMusiclistModel)
        return;

    TPlaylistItem *playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        QClipboard *clipBoard = qApp->clipboard();
        QJsonDocument playlistDocument;
        QJsonArray musicItemsArray;
        QMimeData *mimeData = new QMimeData;
        foreach (int row, rows) {
            TMusicItem *musicItem = playlistItem->musicItem(row);
            if(musicItem)
                musicItemsArray.append(musicItem->toJson());
        }
        playlistDocument.setArray(musicItemsArray);
        mimeData->setData(TMusicItem::mimeType(), playlistDocument.toJson());
        clipBoard->setMimeData(mimeData);
    }
}

void TPlaylistController::slotRequestCutMusicItem(QSet<int> rows)
{
    if(!mMusiclistModel)
        return;

    TPlaylistItem *playlistItem = mMusiclistModel->playlistItem();
    if(playlistItem)
    {
        QClipboard *clipBoard = qApp->clipboard();
        foreach (int row, rows) {
            TMusicItem *musicItem = playlistItem->musicItem(row);
            if(musicItem)
            {
                QMimeData *mimeData = new QMimeData;
                //mimeData->setData(TMusicItem::mimeType(), musicItem->toJson());
                clipBoard->setMimeData(mimeData);
            }
        }
    }
}

void TPlaylistController::slotRequestPasteMusicItem(int pos)
{
    if(!mMusiclistModel)
        return;

    TMusicItems musicItems;
    const QMimeData *mimeData = qApp->clipboard()->mimeData();
    if(mimeData->hasFormat(TMusicItem::mimeType())) {
        QJsonArray musicItemsArray = QJsonDocument::fromJson(mimeData->data(TMusicItem::mimeType())).array();
        for(int i=0;i<musicItemsArray.size();i++)
        {
            TMusicItem *musicItem = new TMusicItem;
            musicItem->fromJson(musicItemsArray.at(i).toObject());
            musicItems.append(musicItem);
        }
    } else if (mimeData->hasFormat(TTrackItem::mimeType())) {
        QJsonArray trackItemsArray = QJsonDocument::fromJson(mimeData->data(TTrackItem::mimeType())).array();
        for(int i=0;i<trackItemsArray.size();i++)
        {
            TMusicItem *musicItem = new TMusicItem;
            TTrackItem *trackItem = new TTrackItem;
            trackItem->fromJson(trackItemsArray.at(i).toObject());
            musicItem->fromTrackItem(trackItem);
            musicItem->addTrackItem(trackItem);
            musicItems.append(musicItem);
        }
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

    TMusicItem *musicItem = mMusiclistModel->playlistItem()->musicItem(row);
    if(musicItem)
    {
        QString fileName = musicItem->fileName();
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

    TMusicItem *musicItem = mMusiclistModel->playlistItem()->musicItem(row);
    if(musicItem)
    {
        QStringList indexList;
        foreach (TTrackItem *trackItem, *musicItem->trackItems()) {
            indexList.append(trackItem->indexName);
        }
        mExportDialog->setMusicFile(musicItem->fileName());
        mExportDialog->setMaxDuration(0);
        mExportDialog->setIndexInfo(indexList);
        if(mExportDialog->getOutputDir().isEmpty())
            mExportDialog->setOutputPath(QFileInfo(musicItem->fileName()).absolutePath());
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


}

void TPlaylistController::slotRequestSortMusiclistAsAlbum()
{
    if(!mMusiclistModel)
        return;

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

    TMusicItem *musicItem = mTracklistModel->musicItem();
    if(musicItem)
    {
        QClipboard *clipBoard = qApp->clipboard();
        QJsonDocument playlistDocument;
        QMimeData *mimeData = new QMimeData;
        QJsonArray trackItemsArray;
        foreach (int row, rows)
        {
            TTrackItem *trackItem = musicItem->trackItem(row);
            if(trackItem)
                trackItemsArray.append(trackItem->toJson());

        }
        playlistDocument.setArray(trackItemsArray);
        mimeData->setData(TTrackItem::mimeType(), playlistDocument.toJson());
        clipBoard->setMimeData(mimeData);
    }
}

void TPlaylistController::slotRequestExportTrackItem(int row)
{
    if(!mTracklistModel || !mExportDialog || !mExportMissionDialog)
        return;

    TMusicItem *musicItem = mTracklistModel->musicItem();
    if(musicItem)
    {
        TTrackItem *trackItem = musicItem->trackItem(row);
        if(trackItem)
        {
            mExportDialog->setMusicFile(musicItem->fileName());
            mExportDialog->setMaxDuration(trackItem->duration);
            mExportDialog->setIndexInfo(trackItem->indexName);
            if(mExportDialog->getOutputDir().isEmpty())
                mExportDialog->setOutputPath(QFileInfo(musicItem->fileName()).absolutePath());
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
    if(mPlaylistModel && mMusiclistModel && mTracklistModel && mPlaylistCore)
    {
        mPlaylistModel->setPlayingIndex(pi);

        if(mPlaylistCore->currentPlaylistItem() == mMusiclistModel->playlistItem())
        {
            mMusiclistModel->setPlayingIndex(mi);
            if(mTracklistModel->musicItem() == mPlaylistCore->currentMusicItem())
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
        foreach (QSharedMemory *m, mExportMissions) {
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
            foreach (QSharedMemory *m, mExportMissions) {
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
        TPlaylistItem *playlistItem = mMusiclistModel->playlistItem();
        if(playlistItem)
        {
            int musicCount = playlistItem->size();
            TMusicItem *musicItem = playlistItem->musicItem(mCurrentViewRow);
            if(musicItem)
            {
                mPropertyDialog->setMusicFile(musicItem->fileName());
                mPropertyDialog->setTitle(musicItem->displayName());
                mPropertyDialog->setAuthor(musicItem->artist());
                mPropertyDialog->setYear(musicItem->year());
                mPropertyDialog->setSystem(musicItem->system());
                mPropertyDialog->setAddionalInfo(musicItem->additionalInfo());
                mPropertyDialog->setIndex(mCurrentViewRow+1, musicCount);
            }
        }
    } else {
        TMusicItem *musicItem = mTracklistModel->musicItem();
        if(musicItem)
        {
            int trackCount = musicItem->size();
            TTrackItem *trackItem = musicItem->trackItem(mCurrentViewRow);
            if(trackItem)
            {
                mPropertyDialog->setMusicFile(musicItem->fileName());
                mPropertyDialog->setTitle(trackItem->displayName);
                mPropertyDialog->setAuthor(trackItem->artist);
                mPropertyDialog->setYear(trackItem->year);
                mPropertyDialog->setSystem(trackItem->system);
                mPropertyDialog->setAddionalInfo(trackItem->additionalInfo);
                mPropertyDialog->setIndex(mCurrentViewRow+1, trackCount);
            }
        }
    }
}
