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
#include "exportcontroller.h"

#include <QUuid>
#include <QProcess>

#include "preferences.h"
#include "utils.h"

#define LOCK(x) QMutexLocker locker(x);\
    Q_UNUSED(locker)

#define PROCESS_TIMEOUT 3000

TExportController::TExportController(QObject *parent) :
    TAbstractController(parent)
  , mMissionsModel(new TMissionsModel(this, &mExportMissions, &mExportMissionsLock))
  , mExportMissionDialog(NULL)
{

}

TExportController::~TExportController()
{
    LOCK(&mExportMissionsLock);
    for(QSharedMemory *m : mExportMissions) {
        TExportParam *exportParam = (TExportParam*)m->data();
        if(exportParam)
            exportParam->state = ES_COMPLETE;

        m->detach();
        m->deleteLater();
    }
    mExportMissions.clear();
}

bool TExportController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    mExportMissionDialog = gui->exportMissionDialog();
    mExportMissionDialog->setModel(mMissionsModel);

    connect(mExportMissionDialog,
            SIGNAL(requestRemoveMissions(QList<int>)),
            this,
            SLOT(slotRequestCancelMissions(QList<int>)));
    connect(mExportMissionDialog,
            SIGNAL(requestPauseMissions(QList<int>)),
            this,
            SLOT(slotRequestPauseMissions(QList<int>)));
    connect(mExportMissionDialog,
            SIGNAL(requestStartMissions(QList<int>)),
            this,
            SLOT(slotRequestStartMissions(QList<int>)));
    connect(mExportMissionDialog,
            SIGNAL(queryMissionsStatus(QList<int>, QList<int>&)),
            this,
            SLOT(slotQueryMissionsStatus(QList<int>,QList<int>&)));
    connect(mExportMissionDialog,
            SIGNAL(requestExploreFiles(QList<int>)),
            this,
            SLOT(slotRequestExploreFiles(QList<int>)));
    connect(mExportMissionDialog,
            SIGNAL(requestRestartMissions(QList<int>)),
            this,
            SLOT(slotRequestRestartMissions(QList<int>)));
    connect(mExportMissionDialog,
            SIGNAL(requestCloseWindow()),
            this,
            SLOT(slotRequestCloseWindow()));

    return TAbstractController::joint(gui, core);
}

bool TExportController::hasExportingMissions()
{
    LOCK(&mExportMissionsLock);

    for(QSharedMemory *m : mExportMissions) {
        TExportParam *exportParam = (TExportParam*)m->data();
        if(exportParam->state==ES_RUNNING || exportParam->state==ES_STARTING || (exportParam->state==ES_PAUSED && exportParam->oldState!=ES_READY))
        {
            exportParam->serverTick = QTime::currentTime().msecsSinceStartOfDay();
            if(exportParam->serverTick-exportParam->clientTick < PROCESS_TIMEOUT)
            {
                return true;
            }
        }
    }

    return false;
}

void TExportController::slotRequestAddExportMissions(void *missions, int size)
{
    if(!missions || size<1 || !mExportMissionDialog)
        return;

    TExportMissions newMissions;
    for(int i=0;i<size;i++)
    {
        TExportParam *exportParam = &((TExportParam*)missions)[i];
        QString strId = QUuid::createUuid().toString();
        QSharedMemory *sharedMemory = new QSharedMemory(strId, this);
        if(!sharedMemory->create(sizeof(TExportParam)))
            continue;
        sharedMemory->attach();
        exportParam->state = ES_READY;
        memcpy(sharedMemory->data(), exportParam, sizeof(TExportParam));
        newMissions.append(sharedMemory);
    }

    mMissionsModel->addMissions(newMissions);

    LOCK(&mExportMissionsLock);
    if(mExportMissions.size() > 0)
        startMyTimer(300);

    mExportMissionDialog->showNormal();
}

void TExportController::slotRequestCancelMissions(QList<int> rows)
{
    if(mMissionsModel)
        mMissionsModel->removeMissions(rows);
}

void TExportController::slotRequestStartMissions(QList<int> rows)
{
    if(mMissionsModel)
        mMissionsModel->startMissions(rows);
}

void TExportController::slotRequestPauseMissions(QList<int> rows)
{
    if(mMissionsModel)
        mMissionsModel->pauseMissions(rows);
}

void TExportController::slotRequestExploreFiles(QList<int> rows)
{
    QStringList fileNames;
    LOCK(&mExportMissionsLock);
    int size = mExportMissions.size();
    for(int i : rows)
    {
        if(i<0 || i>=size)
            continue;

        TExportParam *exportParam = (TExportParam*)mExportMissions[i]->data();
        if(!exportParam)
            continue;

        fileNames.append(QString::fromWCharArray(exportParam->outputPath));
    }

    for(QString fileName : fileNames)
    {
        Utils::exploreFile(fileName);
    }
}

void TExportController::slotRequestRestartMissions(QList<int> rows)
{
    if(mMissionsModel)
        mMissionsModel->restartMissions(rows);
}

void TExportController::slotQueryMissionsStatus(QList<int> rows, QList<int> &state)
{
    LOCK(&mExportMissionsLock);
    int size = mExportMissions.size();
    for(int i : rows)
    {
        if(i<0 || i>=size)
            continue;

        TExportParam *exportParam = (TExportParam*)mExportMissions[i]->data();
        if(!exportParam)
            state.append(ES_NULL);
        else
            state.append(exportParam->state);
    }
}

void TExportController::slotRequestCloseWindow()
{
    if(mMissionsModel)
        mMissionsModel->clearCompletedMissions();
}

void TExportController::slotTimerEvent()
{
    if(mMissionsModel && mExportMissionDialog && mExportMissionDialog->isVisible())
    {
        LOCK(&mExportMissionsLock);

        // Tick check
        int runningCount = 0;
        int completeCount = 0;
        for(QSharedMemory *m : mExportMissions) {
            TExportParam *exportParam = (TExportParam*)m->data();
            if(exportParam->state==ES_RUNNING || exportParam->state==ES_STARTING || (exportParam->state==ES_PAUSED && exportParam->oldState!=ES_READY))
            {
                exportParam->serverTick = QTime::currentTime().msecsSinceStartOfDay();
                if(exportParam->serverTick-exportParam->clientTick > PROCESS_TIMEOUT)
                {
                    if(exportParam->state == ES_STARTING)
                    {
                        // Starting over time
                        Utils::cpy2wchar(exportParam->errorString, tr("Process can not be started"));
                        exportParam->state = ES_ERROR;
                    } else {
                        // Client process has no responce, restart it
                        //exportParam->overwrite = true;
                        //exportParam->state = ES_READY;
                    }
                } else {
                    runningCount++;
                }
            }
            else if(exportParam->state == ES_COMPLETE)
                completeCount++;
        }

        // Maximize 3 processes
        int newSpawn = TPreferences::instance()->exportProcesses()-runningCount;
        if(newSpawn > 0)
        {
            QString commandLine = "exportor ";
            commandLine.prepend("noconsole ");
            for(QSharedMemory *m : mExportMissions) {
                TExportParam *exportParam = (TExportParam*)m->data();
                if(exportParam && exportParam->state == ES_READY)
                {
                    QString runcmd = commandLine+m->key();
                    if(!QProcess::startDetached(runcmd))
                    {
                        Utils::cpy2wchar(exportParam->errorString, tr("Failed to start process with command line, %1").arg(runcmd));
                        exportParam->state = ES_ERROR;
                    } else {
                        exportParam->serverTick = QTime::currentTime().msecsSinceStartOfDay();
                        exportParam->clientTick = exportParam->serverTick;
                        exportParam->state = ES_STARTING;
                    }
                    qDebug() << commandLine+m->key();
                    newSpawn--;
                    if(newSpawn <= 0)
                        break;
                }
            }
        } else {
            if(completeCount >= mExportMissions.size())
                stopMyTimer();
        }

        mMissionsModel->update();

        if(mExportMissions.size() > 0)
            mExportMissionDialog->setWindowTitle(tr("Export sound track(%1/%2)").arg(completeCount).arg(mExportMissions.size()));
    }
    if(mMissionsModel && TPreferences::instance()->autoClearExportMissions())
        mMissionsModel->clearCompletedMissions();
}
