#include "cmdlineparser.h"

#include <QFileInfo>

QSharedMemory *TCmdlineParser::mSharedMemory=NULL;
TExportParam *TCmdlineParser::mExportParam=NULL;

TCmdlineParser::TCmdlineParser(QStringList arguments) :
    QCommandLineParser()
  , mIndexName("")
  , mSourceFile("")
  , mDestFilePath("")
  , mFormat("")
  , mSampleRate(44100)
  , mDuration(0)
  , mOverWrite(false)
  , mVerbose(false)
  , mAsDaemon(false)
  , mIsError(false)
  , mNeedHelp(false)
{
    mArguments = arguments;

    initialize();
}

TCmdlineParser::~TCmdlineParser()
{
    if(mSharedMemory)
    {
        mSharedMemory->detach();
    }
}

TExportParam *TCmdlineParser::getExportParam()
{
    return mExportParam;
}

void TCmdlineParser::initialize()
{
    addOption({{"l", "list"}, tr("List tracks of music.")});
    addOption({{"i", "index"}, tr("Index name of track."), "index name"});
    addOption({{"f", "format"}, tr("Export file format: wave,mp3,pcm, default value is \"wave\"."), "format"});
    addOption({{"d", "duration"}, tr("Specify the duration of track to export.The default duration will be used if this parameter is invalid."), "micro seconds"});
    addOption({{"r", "sample_rate"}, tr("Specify the sample rate, default is 44100HZ."), "sample rate", "44100"});
    addOption({{"y", "over_write"}, tr("Over write existed files.")});
    addOption({{"x", "verbose"}, tr("Displays verbose information.")});

    addPositionalArgument("source", tr("The music file full name to export."));
    addPositionalArgument("destination", tr("The destination file path to export."), "[filepath]");
    addHelpOption();
    addVersionOption();
    setApplicationDescription(tr("Export video game sound track to pcm,wave,mp3 file.Copyright(c) Bilge Theall"));

    process(mArguments);

    if(isSet("i"))
        mIndexName = value("i");

    if(isSet("d"))
        mDuration = value("d").toInt();

    mListMode = isSet("l");

    mSampleRate = value("r").toInt();

    mFormat = value("f");

    mOverWrite = isSet("y");
    mVerbose = isSet("x");

    QStringList arguments = positionalArguments();
    int argsCount = arguments.count();
    if(argsCount > 0)
    {
        mSourceFile = arguments.takeFirst();
        if(!arguments.isEmpty())
        {
            mDestFilePath = arguments.takeFirst();
        }
    } else {
        mNeedHelp = true;
    }

    QFileInfo fi(mSourceFile);
    if(!fi.isFile())
    {
        if(mSourceFile.isEmpty())
            mIsError = true;
        else
        {
            mSharedMemory = new QSharedMemory(mSourceFile);
            if(!mSharedMemory->create(1))
            {
                mSharedMemory->attach();
                mSharedMemory->lock();
                mExportParam = (TExportParam*)mSharedMemory->data();
                mSourceFile = QString::fromWCharArray(mExportParam->fileName);
                mIndexName = QString::fromWCharArray(mExportParam->indexName);
                mDestFilePath = QString::fromWCharArray(mExportParam->outputPath);
                mFormat = mExportParam->format;
                mSampleRate = mExportParam->sampleRate;
                mDuration = mExportParam->duration;
                mOverWrite = mExportParam->overwrite;
                mSharedMemory->unlock();
                mAsDaemon = true;
            } else {
                mIsError = true;
            }
        }
    }
}

