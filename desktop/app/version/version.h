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
#ifndef VERSION_H
#define VERSION_H

#include "pch.h"

#ifdef Q_OS_WIN32
    #include <windows.h>
#endif

class TVersionInfo
{
public:
    explicit TVersionInfo(QString fileName);
    ~TVersionInfo();

    static TVersionInfo *instance(QString fileName=QString());
    static void deleteInstance();

    QString domain();
    QString companyName();
    QString fileDescription();
    QString fileVersion();
    QString internalName();
    QString legalCopyright();
    QString originalFilename();
    QString productName();
    QString productVersion();
    QString compilePlatform();
    QString buildNumber();
    QString buildTime();

private:
    int mVerMajor;
    int mVerSecond;
    int mVerMin;
    int mBuildNo;

    QString mDomain;
    QString mCompanyName;
    QString mFileDescription;
    QString mFileVersion;
    QString mInternalName;
    QString mLegalCopyright;
    QString mOriginalFilename;
    QString mProductName;
    QString mProductVersion;
    QString mCompilePlatform;
    QString mBuildNumber;
    QString mBuildTime;

    static TVersionInfo *mInstance;
    void read(QString fileName);
#ifdef Q_OS_WIN32
    QString value(char *lpData, QString section, LPVOID lpBuffer, UINT uLen);
#endif
};

#endif
