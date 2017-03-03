#include <QCoreApplication>
#include <QSharedMemory>

#include "../../app/core/core.h"
#include "export/exportfactory.h"
#include "commandline/cmdlineparser.h"

#define trp(x) qPrintable(QCoreApplication::translate("main", x))

typedef void (*IProgressCallback)(int value, int max);

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
        TBackendPlugin *plugin,
        TTrackInfo *trackInfo,
        QString outputFile,
        QString format,
        TExportParam *exportParam = NULL
        )
{
    if(!plugin || !trackInfo)
        return false;

    if(!plugin->openTrack(trackInfo))
    {
        printf(trp("Failed to load track \"%s\" in %s\n"), trackInfo->indexName.c_str(), trackInfo->musicFileName.c_str());
        return false;
    }
    TExportFactory exportFactory;
    TAbstractExport *exportor = NULL;
    wstring fileNameW = outputFile.toStdWString();
    if(format.isEmpty())
        exportor = exportFactory.create(fileNameW.c_str(), trackInfo->sampleRate);
    else
        exportor = exportFactory.create(format, fileNameW.c_str(), trackInfo->sampleRate);

    if(!exportor->isOpened())
    {
        printf(trp("Failed to create exporter of %s\n"), qPrintable(outputFile));
        return false;
    }
    int samples = plugin->getSampleSize(trackInfo->sampleRate, SOUND_FPS);
    if(samples < 2)
        samples = 4096;

    int bufSize = samples*4;
    byte *buf = (byte*)malloc(bufSize);
    int silentFrames = 0;
    float framesPerSec = trackInfo->sampleRate*4/bufSize;

    int realDuration = trackInfo->duration;
    if(realDuration < 1)
        realDuration = 0x7fffffff;

    int totalFrames = framesPerSec*realDuration/1000 + 0.5;
    TRequestSamples nextframe = plugin->getCallback();
    if(exportParam)
        exportParam->progressTotalFrames = totalFrames;

    for(int i=0;i<totalFrames;i++)
    {
        if(exportParam)
        {
            while(exportParam->state!=ES_RUN)
                QThread::msleep(500);
        }
        memset(buf, 0, bufSize);
        nextframe(bufSize, buf);
        if(isBlankFrame(buf, bufSize))
        {
            silentFrames++;
        } else {
            silentFrames = 0;
        }
        if(silentFrames >= framesPerSec*3){
            // Frames end
            break;
        }
        exportor->write((const byte*)buf, bufSize);
        if(exportParam)
            exportParam->progressCurrentFrames = i+1;
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
        printf(trp("Music file path is invalid or non-exists.\n"));
        return 0;
    }
    // Load backend plugins
    TBackendPluginManager *plugins = TBackendPluginManager::instance();
    QString indexName = parser.indexName();
    QString sourceFile = parser.sourceFile();
    QString destFilePath = parser.destFilePath();
    QString format = parser.format();

    int sampleRate = parser.sampleRate();
    int duration = parser.duration();
    bool overWriteFile = parser.overWrite();
    bool bVerbose = parser.verbose();

    TExportParam *exportParam = NULL;
    if(parser.runAsDaemon())
        exportParam = TCmdlineParser::getExportParam();

    // Parse track list
    TMusicInfo musicInfo;
    TBackendPlugin *plugin = plugins->parse(sourceFile, &musicInfo);
    if(!plugin)
    {
        printf(trp("Music file can not be parsed.\n"));
        return 0;
    }

    int trackCount = musicInfo.trackList.size();
    if(trackCount == 0)
    {
        printf(trp("No tracks in music file.\n"));
        return 0;
    } else if(bVerbose) {
        printf(trp("Find %d tracks in music file.\n"), trackCount);
    }

    TTrackInfoList trackList;
    if(!indexName.isEmpty())
    {
        TTrackInfo *trackTarget = NULL;
        for(int i=0;i<trackCount;i++)
        {
            TTrackInfo *trackPointer = musicInfo.trackList[i];
            if(trackPointer->indexName == indexName.toStdString())
            {
                trackTarget = trackPointer;
                break;
            }
        }

        if(trackTarget)
            trackList.push_back(trackTarget);
        else
        {
            printf(trp("Failed to find specified track with index %s\n"), qPrintable(indexName));
            return 0;
        }
    } else {
        trackList = musicInfo.trackList;
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
    int trackListSize = trackList.size();

    int nSkip = 0;
    int nSuccess = 0;
    int nFail = 0;
    for(int i=0;i<trackListSize;i++)
    {
        TTrackInfo *trackInfo = trackList[i];
        trackInfo->musicFileName = sourceFile.toStdString();
        trackInfo->sampleRate = sampleRate;
        if(trackInfo->duration <= 0)
            trackInfo->duration = duration;
        QString destFileFullName;
        if((trackListSize>1 && indexName.isEmpty()) || destIsDir)
        {
            // While exporting muliple tracks, auto set dest name to track name
            QString baseName = QFileInfo(QString::fromStdString(trackInfo->trackName)).baseName()+".wav";
            destFileFullName = destDir.absoluteFilePath(baseName);
        } else {
            destFileFullName = destFilePath;
        }

        if(QFileInfo(destFileFullName).exists() && !overWriteFile)
        {
            if(bVerbose)
                printf(trp("Warning, destination file exists, %s\n"), qPrintable(destFileFullName));

            nSkip++;
            continue;
        }
        if(exportTrack(plugin, trackInfo, destFileFullName, format, exportParam))
        {
            nSuccess++;
            printf(trp("Export to %s\n"), qPrintable(destFileFullName));
        } else {
            nFail++;
        }
    }
    if(trackListSize <= 0)
    {
        printf(trp("No tracks exported.\n"));
    } else if(trackListSize > 1) {
        printf(trp("Success: %d, Failed: %d, Skipped: %d.\n"), nSuccess, nFail, nSkip);
    }

    a.quit();
    return 0;
}

