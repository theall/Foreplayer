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
#ifndef TAPP_H
#define TAPP_H

#include <QApplication>
#include <QSharedMemory>

#include "controller/maincontroller.h"

class TCheckThread : public QThread
{
    Q_OBJECT

public:
    explicit TCheckThread(TGuiManager *gui);
    ~TCheckThread();

signals:
    void requestRestoreGui();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    TGuiManager *mGui;
    QSharedMemory *mShareMemory;
};

class TApp
{
public:
    explicit TApp(int argc, char *argv[]);
    ~TApp();

    int start();
    bool isRunning();

private:
    QApplication *mApp;
    TCheckThread *mCheckThread;
};

#endif // TAPP_H
