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
#include "version.h"

#ifdef Q_OS_WIN32
    #include "win32_ver.h"
#endif

TVersionInfo *TVersionInfo::mInstance = NULL;

TVersionInfo::TVersionInfo(QString fileName) :
    mVerMajor(0),
    mVerSecond(0),
    mVerMin(0),
    mBuildNo(0)
{
    read(fileName);
}

TVersionInfo::~TVersionInfo()
{

}

TVersionInfo *TVersionInfo::instance(QString fileName)
{
    if(!mInstance)
        mInstance = new TVersionInfo(fileName);

    return mInstance;
}

void TVersionInfo::deleteInstance()
{
    if(mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

QString TVersionInfo::companyName()
{
    return mCompanyName;
}

QString TVersionInfo::fileDescription()
{
    return mFileDescription;
}

QString TVersionInfo::fileVersion()
{
    return mFileVersion;
}

QString TVersionInfo::internalName()
{
    return mInternalName;
}

QString TVersionInfo::legalCopyright()
{
    return mLegalCopyright;
}

QString TVersionInfo::originalFilename()
{
    return mOriginalFilename;
}

QString TVersionInfo::productName()
{
    return mProductName;
}

QString TVersionInfo::productVersion()
{
    return mProductVersion;
}

QString TVersionInfo::compilePlatform()
{
    return mCompilePlatform;
}

QString TVersionInfo::buildNumber()
{
    return mBuildNumber;
}

QString TVersionInfo::buildTime()
{
    return mBuildTime;
}

QString TVersionInfo::domain()
{
    return mDomain;
}

void TVersionInfo::read(QString fileName)
{
#ifdef Q_OS_WIN32
    DWORD dwLen = 0;
    char* lpData=NULL;

    BOOL bSuccess = FALSE;

    //获得文件基础信息
    //--------------------------------------------------------
    dwLen = GetFileVersionInfoSize(fileName.toStdWString().c_str(), 0);

    if (0 == dwLen)
    {
        qDebug() << "Get file version size error!";
        return;
    }
    lpData = new char[dwLen+1];

    bSuccess = GetFileVersionInfo(fileName.toStdWString().c_str(), 0, dwLen, lpData);
    if (!bSuccess)
    {
        qDebug() << "Get file version info error!";
        delete lpData;
        return;
    }

    LPVOID lpBuffer = NULL;
    UINT uLen = 0;

    QString code = "\\VarFileInfo\\Translation";
    bSuccess = VerQueryValue(lpData,
                (code.toStdWString().c_str()),
                &lpBuffer,
                &uLen);
    if (!bSuccess)
    {
        qDebug() << "Get translation error!";
        delete lpData;
        return;
    }

    QString strTranslation,str1,str2;
    unsigned short int *p =(unsigned short int *)lpBuffer;
    str1.setNum(*p, 16);
    str1="000" + str1;
    strTranslation+= str1.mid(str1.size()-4,4);
    str2.setNum(*(++p),16);
    str2="000" + str2;
    strTranslation+= str2.mid(str2.size()-4,4);

    QString sectionHead = "\\StringFileInfo\\" + strTranslation + "\\";
    mCompanyName = value(lpData, sectionHead+K_COMPANYNAME, lpBuffer, uLen);
    mFileDescription = value(lpData, sectionHead+K_FILEDESCRIPTION, lpBuffer, uLen);
    mFileVersion = value(lpData, sectionHead+K_FILEVERSION, lpBuffer, uLen);
    mInternalName = value(lpData, sectionHead+K_INTERNALNAME, lpBuffer, uLen);
    mLegalCopyright = value(lpData, sectionHead+K_LEGALCOPYRIGHT, lpBuffer, uLen);
    mOriginalFilename = value(lpData, sectionHead+K_ORIGINALFILENAME, lpBuffer, uLen);
    mProductName = value(lpData, sectionHead+K_PRODUCTNAME, lpBuffer, uLen);
    mProductVersion = value(lpData, sectionHead+K_PRODUCTVERSION, lpBuffer, uLen);
    mCompilePlatform = value(lpData, sectionHead+K_COMPILEPLATFORM, lpBuffer, uLen);
    mBuildNumber = value(lpData, sectionHead+K_BUILDNUMBER, lpBuffer, uLen);
    mBuildTime = value(lpData, sectionHead+K_BUILD_TIME, lpBuffer, uLen);
    mDomain = value(lpData, sectionHead+K_DOMAIN, lpBuffer, uLen);

    delete[] lpData;
    return;
#endif
}

#ifdef Q_OS_WIN32
QString TVersionInfo::value(char *lpData, QString section, LPVOID lpBuffer, UINT uLen)
{
    bool bSuccess = VerQueryValue(lpData,
                (section.toStdWString().c_str()),
                &lpBuffer,
                &uLen);
    if (!bSuccess)
    {
        qDebug() << "Read section error:" << section;
        return QString();
    }

    return QString::fromUtf16((const unsigned short int *)lpBuffer);
}

#endif
