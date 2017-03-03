#ifndef TCMDLINEPARSER_H
#define TCMDLINEPARSER_H

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QSharedMemory>
#include "utils.h"

class TCmdlineParser : public QCommandLineParser
{
public:
    TCmdlineParser(QStringList arguments);
    ~TCmdlineParser();

    static TExportParam *getExportParam();

    QString indexName() { return mIndexName; }
    QString sourceFile() { return mSourceFile; }
    QString destFilePath() { return mDestFilePath; }
    QString format() { return mFormat; }
    int sampleRate() { return mSampleRate; }
    int duration() { return mDuration; }
    bool overWrite() { return mOverWrite; }
    bool verbose() { return mVerbose; }
    bool runAsDaemon() { return mAsDaemon; }
    bool isError() { return mIsError; }
    bool isListMode() { return mListMode; }
    bool needHelp() { return mNeedHelp; }

private:
    QString mIndexName;
    QString mSourceFile;
    QString mDestFilePath;
    QString mFormat;
    int mSampleRate;
    int mDuration;
    bool mOverWrite;
    bool mVerbose;
    bool mAsDaemon;
    bool mIsError;
    bool mNeedHelp;
    bool mListMode;
    QStringList mArguments;
    static QSharedMemory *mSharedMemory;
    static TExportParam *mExportParam;
    void initialize();
};

#endif // TCMDLINEPARSER_H
