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
#include "app.h"

#include "version/version.h"
#include "utils/preferences.h"
#include "controller/controllerproxy.h"
#include "core/core.h"

#include <QTextCodec>

#define FLAG_OFF    0x0
#define FLAG_ON     0x1

#define CMD_NULL    0x0
#define CMD_RAISE   0x1
#define CMD_EXIT    0x2

#ifdef QT_DEBUG
#define GLOBAL_SHARE_MEMORY_KEY     "Foreplayerd/Theall"
#else
#define GLOBAL_SHARE_MEMORY_KEY     "Foreplayer/Theall"
#endif

struct PROCESS_CHANNEL
{
    char flag;
    char command;
};

TCheckThread::TCheckThread(TGuiProxy *guiProxy) :
    QThread(guiProxy)
  , mGuiProxy(guiProxy)
  , mShareMemory(new QSharedMemory(GLOBAL_SHARE_MEMORY_KEY))
{
    mShareMemory->create(sizeof(PROCESS_CHANNEL));
    mShareMemory->attach();
    mShareMemory->lock();
    PROCESS_CHANNEL *pc = (PROCESS_CHANNEL*)mShareMemory->data();
    pc->flag = FLAG_ON;
    pc->command = CMD_NULL;
    mShareMemory->unlock();
}

TCheckThread::~TCheckThread()
{
    if(mShareMemory)
    {
        mShareMemory->detach();
        delete mShareMemory;
        mShareMemory = NULL;
    }
}

void TCheckThread::run()
{
    while(true)
    {
        mShareMemory->lock();
        PROCESS_CHANNEL *pc = (PROCESS_CHANNEL*)mShareMemory->data();
        if(pc)
        {
            if(TPreferences::instance()->enableMultiInstance())
                pc->flag = FLAG_OFF;
            else {
                pc->flag = FLAG_ON;
                switch (pc->command) {
                case CMD_RAISE:
                    pc->command = CMD_NULL;
                    emit requestRestoreGui();
                    break;
                case CMD_EXIT:
                    pc->command = CMD_NULL;
                    if(mGuiProxy)
                        mGuiProxy->exit();
                    break;
                case CMD_NULL:
                    break;
                default:
                    break;
                }
            }
        }
        mShareMemory->unlock();
        msleep(500);
    }
}

TApp::TApp(int argc, char *argv[]) :
    mApp(new QApplication(argc, argv))
  , mCheckThread(NULL)
{
    TVersionInfo *vi = TVersionInfo::instance();
    mApp->setOrganizationDomain(vi->legalCopyright());
    mApp->setApplicationDisplayName(vi->fileDescription());
    mApp->setOrganizationName(vi->companyName());
    mApp->setApplicationName(vi->productName());
    mApp->setApplicationVersion(vi->productVersion());

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    TPreferences::instance();
}

TApp::~TApp()
{
    TPreferences::deleteInstance();
}

int TApp::start()
{
    TControllerProxy controller;
    TGuiProxy gui(&controller);
    TCore core;
    if(!controller.joint(&gui, &core))
        return 0;

    core.loadPlayList();
    if(!core.isInitialized())
        gui.setTitle(core.getErrorString());

    if(!mCheckThread)
    {
        mCheckThread = new TCheckThread(&gui);
        QObject::connect(mCheckThread, &TCheckThread::requestRestoreGui, &gui, &TGuiProxy::restoreGui);
        mCheckThread->start();
    }

    gui.open();
    int ret = mApp->exec();
    if(mCheckThread)
    {
        mCheckThread->terminate();
        mCheckThread->wait();
        mCheckThread = NULL;
    }
    return ret;
}

bool TApp::isRunning()
{
    bool ret = false;

    if(!TPreferences::instance()->enableMultiInstance())
    {
        static QSharedMemory data(GLOBAL_SHARE_MEMORY_KEY);
        if(!data.create(sizeof(PROCESS_CHANNEL)))
        {
            data.attach();
            data.lock();
            PROCESS_CHANNEL *pc = (PROCESS_CHANNEL*)data.data();
            if(pc)
            {
                if(pc->flag==FLAG_ON)
                {
                    pc->command = CMD_RAISE;
                    ret = true;
                } else {
                    ret = false;
                }
            }
            data.unlock();
            data.detach();
        }
    }

    return ret;
}
