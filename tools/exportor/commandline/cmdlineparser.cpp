#include "cmdlineparser.h"

#include <QFileInfo>

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
{
    mArguments = arguments;

    initialize();
}

TCmdlineParser::~TCmdlineParser()
{

}

bool TCmdlineParser::exportSingle()
{
    return !mIndexName.isEmpty();
}

void TCmdlineParser::initialize()
{
    addOption({{"l", "list"}, tr("List tracks of music.")});
    addOption({{"i", "index"}, tr("Index name of track."), "index name"});
    addOption({{"f", "format"}, tr("Export file format: wave|mp3, default is wave."), "format"});
    addOption({{"d", "duration"}, tr("Specify the duration of track to export.The default duration will be used if this parameter is invalid."), "micro seconds"});
    addOption({{"r", "sample_rate"}, tr("Specify the sample rate, default is 44100HZ."), "sample rate", "44100"});
    addOption({{"y", "over_write"}, tr("Over write existed files.")});
    addOption({{"x", "verbose"}, tr("Display verbose information.")});

    addPositionalArgument("source", tr("The music file full name to export."));
    addPositionalArgument("destination", tr("The destination file path to export."), "[filepath]");
    addHelpOption();
    addVersionOption();
    setApplicationDescription(tr("Export video game sound track to wave/mp3 file."));

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
        mIsError = true;
    }

    QFileInfo fi(mSourceFile);
    if(!fi.isFile())
    {
        if(mSourceFile.isEmpty())
            mIsError = true;
        else
            mAsDaemon = true;
    }
}

