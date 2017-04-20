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
#ifndef UTILS_H
#define UTILS_H

#include "pch.h"

enum ExportState
{
    ES_NULL,
    ES_READY,
    ES_RUN,
    ES_ERROR,
    ES_PAUSED,
    ES_COMPLETE
};

struct TExportParam
{
    wchar_t fileName[260];
    wchar_t indexName[260];
    wchar_t outputPath[260];
    wchar_t errorString[512];
    char format[10];
    int sampleRate;
    int duration;
    bool autoTruncate;
    bool overwrite;
    int progressCurrentFrames;
    int progressTotalFrames;
    ExportState state;

    TExportParam()
    {
        *fileName = 0;
        *indexName = 0;
        *outputPath = 0;
        *errorString = 0;
        *format = 0;
        sampleRate = 44100;
        duration = 0;
        autoTruncate = false;
        overwrite = false;
        progressCurrentFrames = 0;
        progressTotalFrames = 0;
        state = ES_NULL;
    }
};

namespace Utils{
    const int TextHighlight = Qt::UserRole + 1;
    const int IsCurrentRow = TextHighlight + 1;
    const int IsPlayingItem = IsCurrentRow + 1;

    QString microSecToTimeStr(long ms, bool padZero=true);
    QString secToTimeStr(long seconds, bool padZero=true);
    QString absoluteFilePath(QString fileName);
}

#endif // UTILS_H
