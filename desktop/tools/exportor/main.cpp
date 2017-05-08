#include <QCoreApplication>
#include <QTime>

#include "../../app/core/core.h"
#include "export/exportfactory.h"
#include "commandline/cmdlineparser.h"

#define TICK_CHECK_TIME 3000
#define tr(x) QCoreApplication::translate("main", x)

#define CHECK_TICK \
    exportParam->clientTick = QTime::currentTime().msecsSinceStartOfDay();\
    if(exportParam->clientTick-exportParam->serverTick > TICK_CHECK_TIME)\
        return false

#define CHECK_STATE(x) \
    while(exportParam->state!=x)\
    {\
        if(exportParam->state == ES_COMPLETE)\
            return false;\
        CHECK_TICK;\
        QThread::msleep(500);\
    }

typedef void (*IProgressCallback)(int value, int max);

struct TrackItemParam
{
    QString sourceFile;
    int sampleRate = 0;
    int duration = 0;
};

void printError(QString error, TExportParam *param=NULL)
{
    printf(qPrintable(error));
    if(param)
    {
        wstring w = error.toStdWString();
        wcscpy(param->errorString, w.c_str());
        param->state = ES_ERROR;
    } else {
        printf("\n");
    }
}

bool isBlankFrame(byte *buf, int size)
{
    short *pBuf = (short*)buf;
    int validPcm = 0;
    int sumFrame = 0;
    for(int i=0;i<size/2;i++)
    {
        short amp = abs(*pBuf);
        if(amp > 16)
            validPcm++;

        sumFrame += amp;
        pBuf++;
    }
    return sumFrame==0 || (float)validPcm/size<0.05;
}

bool exportTrack(
        TCore *core,
        TrackItem trackItem,
        TrackItemParam *param,
        QString outputFile,
        QString format,
        TExportParam *exportParam = NULL
        )
{
    if(!core || !trackItem)
        return false;

    if(!core->loadTrackItem(trackItem))
    {
        printError(tr("Failed to load track %1 in %2").arg(core->getTrackItemIndexName(trackItem)).arg(param->sourceFile), exportParam);
        return false;
    }
    TExportFactory exportFactory;
    TAbstractExport *exportor = NULL;
    wstring fileNameW = outputFile.toStdWString();
    if(format.isEmpty())
        exportor = exportFactory.create(fileNameW.c_str(), param->sampleRate);
    else
        exportor = exportFactory.create(format, fileNameW.c_str(), param->sampleRate);

    if(!exportor->isOpened())
    {
        printError(tr("Failed to open %1").arg(outputFile), exportParam);
        return false;
    }
    if(TMP3Export *mp3Exportor=reinterpret_cast<TMP3Export*>(exportor))
    {
        mp3Exportor->setTitle(core->getTrackItemName(trackItem).toStdString());
        mp3Exportor->setArtist(core->getTrackItemArtist(trackItem).toStdString());
        mp3Exportor->setAlbum(core->getTrackItemAlbum(trackItem).toStdString());
        mp3Exportor->setYear(core->getTrackItemYear(trackItem).toInt());
        mp3Exportor->setComment(core->getTrackItemAdditionalInfo(trackItem).toStdString());
    }
    int samples = core->getFrameSampleCount(param->sampleRate, 600);
    if(samples < 2)
        samples = 4096;

    int bufSize = samples*4;
    byte *buf = (byte*)malloc(bufSize);
    int silentFrames = 0;
    float framesPerSec = param->sampleRate/samples;

    int realDuration = core->getTrackItemDuration(trackItem);
    if(realDuration < 1)
        realDuration = 150000;

    int totalFrames = framesPerSec*realDuration/1000 + 0.5;
    if(exportParam)
    {
        wcscpy(exportParam->outputPath, fileNameW.c_str());
        exportParam->progressTotalFrames = totalFrames;

        CHECK_STATE(ES_STARTING);
        exportParam->state = ES_RUNNING;
    }

    for(int i=0;i<totalFrames;i++)
    {
        if(exportParam)
        {
            CHECK_TICK;
            CHECK_STATE(ES_RUNNING);
        }
        memset(buf, 0, bufSize);
        core->getNextFrame((char*)buf, bufSize);
        if(isBlankFrame(buf, bufSize))
        {
            silentFrames++;
        } else {
            silentFrames = 0;
        }
        if(silentFrames >= framesPerSec*3){
            // Frames end
            if(exportParam)
                exportParam->progressTotalFrames = i;
            break;
        }
        try {
            exportor->write((const byte*)buf, bufSize);
        } catch(...) {
            printError(tr("IO error."), exportParam);
            return false;
        }

        if(exportParam)
            exportParam->progressCurrentFrames = i+1;

#ifdef QT_DEBUG
        QThread::msleep(100);
#else
        QThread::msleep(1);
#endif
    }
    if(exportParam)
        exportParam->state = ES_COMPLETE;
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("Exportor");
    QCoreApplication::setApplicationVersion("1.0.0");

    TCmdlineParser parser(a.arguments());
    if(parser.needHelp())
    {
        parser.showHelp();
        return 0;
    }
    if(parser.isError())
    {
        printError("Music file path is invalid or non-exists.");
        return 0;
    }

    QString indexName = parser.indexName();
    QString sourceFile = parser.sourceFile();
    QString destFilePath = parser.destFilePath();
    QString format = parser.format();

    int sampleRate = parser.sampleRate();
    int duration = parser.duration();
    bool overWriteFile = parser.overWrite();
    bool bVerbose = parser.verbose();
    bool isListMode = parser.isListMode();

    TExportParam *exportParam = NULL;
    if(parser.runAsDaemon())
    {
        exportParam = TCmdlineParser::getExportParam();
        exportParam->clientTick = QTime::currentTime().msecsSinceStartOfDay()+TICK_CHECK_TIME;
    }

    // Load backend plugins
    TCore *core = NULL;
    try
    {
        core = new TCore(true);
    } catch(QString s) {
        printError(s, exportParam);
        return -1;
    }

    // Parse track list
    MusicItem musicItem = core->parse(sourceFile);
    if(!musicItem)
    {
        printError(tr("Music file can not be parsed."), exportParam);
        return 0;
    }

    int trackCount = core->getTrackItemCount(musicItem);
    if(trackCount == 0)
    {
        printError(tr("No tracks in music file."), exportParam);
        return 0;
    } else if(bVerbose) {
        printError(tr("Find %1 tracks in music file.").arg(trackCount));
    }

    TrackItems trackItems;
    if(!indexName.isEmpty())
    {
        TrackItem trackTarget = NULL;
        for(int i=0;i<trackCount;i++)
        {
            TrackItem trackItem = core->getTrackItem(musicItem, i);
            if(core->getTrackItemIndexName(trackItem).toLower() == indexName.toLower())
            {
                trackTarget = trackItem;
                break;
            }
        }
        if(trackTarget)
            trackItems.push_back(trackTarget);
        else
        {
            printError(tr("Failed to find specified track with index %1").arg(indexName), exportParam);
            return 0;
        }
    } else {
        trackItems = core->getTrackItems(musicItem);
    }

    bool destIsDir = true;
    QFileInfo fi(sourceFile);

    if(destFilePath.isEmpty())
    {
        destFilePath = fi.absolutePath();
    } else {
        QFileInfo dfi(destFilePath);
        if(dfi.isRelative())
            destFilePath = fi.absoluteDir().filePath(destFilePath);

        destIsDir = dfi.isDir();
        if(dfi.suffix().isEmpty() && !dfi.exists())
            dfi.absoluteDir().mkpath(destFilePath);
    }

    QDir destDir(destFilePath);
    int trackListSize = trackItems.size();

    int nSkip = 0;
    int nSuccess = 0;
    int nFail = 0;
    for(int i=0;i<trackListSize;i++)
    {
        TrackItem trackItem = trackItems[i];
        QString trackItemName = core->getTrackItemName(trackItem);
        if(isListMode) {
            printError(tr("%1 %2").arg(core->getTrackItemIndexName(trackItem)).arg(trackItemName));
        } else {
            TrackItemParam param;
            param.sampleRate = sampleRate;
            if(core->getTrackItemDuration(trackItem) <= 0)
                param.duration = duration;

            QString destFileFullName;
            if((trackListSize>1 && indexName.isEmpty()) || destIsDir)
            {
                // While exporting muliple tracks, auto set dest name to track name
                QString baseName = trackItemName;
                if(exportParam)
                {
                    if(exportParam->number>0)
                        baseName.prepend(QString("%1.").arg(exportParam->number));
                } else {
                    if(trackListSize > 1)
                        baseName.prepend(QString("%1.").arg(i+1));
                }
                if(QFileInfo(baseName).suffix().toLower() != format.toLower())
                    baseName += "."+format;
                destFileFullName = destDir.absoluteFilePath(baseName);
            } else {
                destFileFullName = destFilePath;
            }
            if(QFileInfo(destFileFullName).exists() && !overWriteFile)
            {
                //if(bVerbose || exportParam)
                printError(tr("Warning, destination file exists, %1").arg(destFileFullName), exportParam);

                nSkip++;
                continue;
            }
            if(exportTrack(core, trackItem, &param, destFileFullName, format, exportParam))
            {
                nSuccess++;
                printError(tr("Export to %1").arg(destFileFullName));
            } else {
                nFail++;
            }
        }

    }
    if(!isListMode)
    {
        if(trackListSize <= 0)
        {
            printError(tr("No tracks exported."));
        } else if(trackListSize > 1) {
            printError(tr("Success: %1, Failed: %2, Skipped: %3.").arg(nSuccess).arg(nFail).arg(nSkip));
        }
    }

    delete core;

    a.quit();
    return 0;
}

