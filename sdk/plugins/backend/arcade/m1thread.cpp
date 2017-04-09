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

#include "m1thread.h"

TM1Thread::TM1Thread()
{
    mThread = new std::thread(&TM1Thread::run, this);
    mThread->detach();
}

TM1Thread::~TM1Thread()
{
    if(mThread)
    {
        delete mThread;
        mThread = NULL;
    }
}

void TM1Thread::run()
{
    m1snd_setoption(M1_OPT_RETRIGGER, 0);
    m1snd_setoption(M1_OPT_WAVELOG, 0);
    m1snd_setoption(M1_OPT_NORMALIZE, 1);
    m1snd_setoption(M1_OPT_LANGUAGE, M1_LANG_EN);
    m1snd_setoption(M1_OPT_RESETNORMALIZE, 0);
    m1snd_setoption(M1_OPT_INTERNALSND, 0);

    //Initial the m1 libary.
    m1snd_init((void*)0, m1ui_message);

    while(0)
    {
        m1snd_run(M1_CMD_IDLE, 0);
        int curgame = m1snd_get_info_int(M1_IINF_CURGAME, 0);
        int current = m1snd_get_info_int(M1_IINF_CURCMD, 0);
        if (m1snd_get_info_int(M1_IINF_TRKLENGTH, (current<<16) | curgame) != -1)
        {
            if (m1snd_get_info_int(M1_IINF_CURTIME, 0) >= m1snd_get_info_int(M1_IINF_TRKLENGTH, (current<<16) | curgame))
            {
                //Playing finished
                //emit playFinished();
            }
        }
        std::this_thread::sleep_for((std::chrono::milliseconds(120)));
    }
    //Close the m1.
    //m1snd_shutdown();
}
