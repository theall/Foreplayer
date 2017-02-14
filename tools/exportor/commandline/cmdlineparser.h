#ifndef TCMDLINEPARSER_H
#define TCMDLINEPARSER_H

#include <QCommandLineOption>
#include <QCommandLineParser>

#define tr(x) QCoreApplication::translate("main", x)

class TCmdlineParser : public QCommandLineParser
{
public:
    TCmdlineParser(QStringList arguments);
    ~TCmdlineParser();

    bool exportSingle();


    QString indexName() { return mIndexName; }
    QString sourceFile() { return mSourceFile; }
    QString destFilePath() { return mDestFilePath; }
    QString format() { return mFormat; }
    int sampleRate() { return mSampleRate; }
    int duration() { return mDuration; }
    bool overWrite() { return mOverWrite; }
    bool verbose() { return mVerbose; }
    bool runningAsDaemon() { return mAsDaemon; }
    bool isError() { return mIsError; }
    bool isListMode() { return mListMode; }

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
    bool mListMode;
    QStringList mArguments;

    void initialize();
};

#endif // TCMDLINEPARSER_H
