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
