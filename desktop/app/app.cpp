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

#include <QTextCodec>

TCheckThread::TCheckThread(TGuiManager *gui) :
    QThread()
  , mGui(gui)
  , mShareMemory(new QSharedMemory(GLOBAL_SHARE_MEMORY_KEY))
{
#ifndef QT_DEBUG
    mShareMemory->create(1);
    mShareMemory->attach();
#endif
}

TCheckThread::~TCheckThread()
{
    if(mShareMemory)
    {
        delete mShareMemory;
        mShareMemory = NULL;
    }
}

void TCheckThread::run()
{
    while(true)
    {
        mShareMemory->lock();
        char *b = (char*)mShareMemory->data();
        if(b && *b && mGui)
        {
            *b = 0;
            mGui->show();
        }
        mShareMemory->unlock();
        msleep(500);
    }
}

TApp::TApp(int argc, char *argv[]) :
    mApp(new QApplication(argc, argv))
  , mCheckThread(NULL)
{
    TVersionInfo *vi = TVersionInfo::instance(QString(argv[0]));
    mApp->setOrganizationDomain(vi->domain());
    mApp->setApplicationName(vi->productName());
    mApp->setApplicationVersion(vi->productVersion());

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    TPreferences::instance();
}

TApp::~TApp()
{
    if(mCheckThread)
    {
        mCheckThread->terminate();
        delete mCheckThread;
        mCheckThread = NULL;
    }

    TPreferences::deleteInstance();
}

int TApp::start()
{
    TMainController controller;
    TGuiManager gui(&controller);
    TCore *core=NULL;
    try
    {
        core = new TCore;
    } catch(QString s) {
        core = NULL;
        gui.mainWindow()->setTitles(s);
    }

    if(!core || !controller.joint(&gui, core))
        return 0;

#ifndef QT_DEBUG
    if(!mCheckThread)
    {
        mCheckThread = new TCheckThread(&gui);
        mCheckThread->start();
    }
#endif

    int ret = mApp->exec();
    delete core;
    return ret;
}

bool TApp::isRunning()
{
#ifndef QT_DEBUG
    static QSharedMemory data(GLOBAL_SHARE_MEMORY_KEY);
    if(!data.create(1))
    {
        data.attach();
        data.lock();
        *(char*)data.data() = 1;
        data.unlock();
        data.detach();
        return true;
    }
#endif
    return false;
}
