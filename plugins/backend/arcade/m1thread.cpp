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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTimer>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>

#include "m1thread.h"

TM1Thread::TM1Thread(QObject *parent):
    QThread(parent)
{

}

void TM1Thread::run()
{
    proc_setoption(M1_OPT_RETRIGGER, 0);
    proc_setoption(M1_OPT_WAVELOG, 0);
    proc_setoption(M1_OPT_NORMALIZE, 1);
    proc_setoption(M1_OPT_LANGUAGE, M1_LANG_EN);
    proc_setoption(M1_OPT_RESETNORMALIZE, 0);
    proc_setoption(M1_OPT_INTERNALSND, 0);

    //Initial the m1 libary.
    proc_init((void*)0, m1ui_message);

    while(0)
    {
        {
            proc_run(M1_CMD_IDLE, 0);

            int curgame = proc_get_info_int(M1_IINF_CURGAME, 0);

            int current = proc_get_info_int(M1_IINF_CURCMD, 0);

            if (proc_get_info_int(M1_IINF_TRKLENGTH, (current<<16) | curgame) != -1)
            {
                if (proc_get_info_int(M1_IINF_CURTIME, 0) >= proc_get_info_int(M1_IINF_TRKLENGTH, (current<<16) | curgame))
                {
                    //Playing finished
                    //emit playFinished();
                }
            }
        }
        msleep(120);
    }

    //Close the m1.
    //proc_shutdown();
}
