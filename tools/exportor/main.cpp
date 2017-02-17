#include <QCoreApplication>
#include <QSharedMemory>

#include "../../app/core/core.h"
#include "export/exportfactory.h"
#include "commandline/cmdlineparser.h"

#define trp(x) qPrintable(QCoreApplication::translate("main", x))

bool isBlankFrame(short *buf, int size)
{
    short *pBuf = buf;
    int validPcm = 0;
    int sumFrame = 0;
    for(int i=0;i<size;i++)
    {
        short amp = abs(*pBuf);
        if(amp > 16)
            validPcm++;

        sumFrame += amp;
        pBuf++;
    }
    return sumFrame==0 || (float)validPcm/size<0.05;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("Exportor");
    QCoreApplication::setApplicationVersion("1.0");

    TCmdlineParser parser(a.arguments());
    if(parser.needHelp())
    {
        parser.showHelp();
        return 0;
    }
    if(parser.isError())
    {
        printf(trp("Music file is invalid or non-exist.\n"));
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

    //QString sourceFile = parser.
    TBackendPluginManager *plugins = TBackendPluginManager::instance();
    TTrackInfo trackInfo;
    trackInfo.musicFileName = sourceFile.toStdString();
    trackInfo.indexName = indexName.toStdString();

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

    TRequestSamples nextframe = plugin->getCallback();

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

    TExportFactory exportFactory;
    int nSkip = 0;
    int nSuccess = 0;
    int nFail = 0;
    for(int i=0;i<trackListSize;i++)
    {
        TTrackInfo *track = trackList[i];
        track->musicFileName = sourceFile.toStdString();
        if(!plugin->openTrack(track))
        {
            printf(trp("Failed to load track, %s\n"), track->indexName.c_str());
            nFail++;
            continue;
        }

        QString destFileFullName;
        if((trackListSize>1 && indexName.isEmpty()) || destIsDir)
        {
            // While export muliple tracks, auto set dest name to track name
            QString baseName = QFileInfo(QString::fromStdString(track->trackName)).baseName()+format;
            destFileFullName = destDir.absoluteFilePath(baseName);
        } else {
            destFileFullName = destFilePath;
        }

        if(QFileInfo(destFileFullName).exists())
        {
            if(bVerbose)
                printf(trp("Warning, destination file exists, %s\n"), qPrintable(destFileFullName));

            if(!overWriteFile)
            {
                nSkip++;
                continue;
            }
        }

        wstring fileNameW = destFileFullName.toStdWString();

        TAbstractExport *exportor = NULL;
        if(format.isEmpty())
            exportor = exportFactory.create(fileNameW.c_str(), sampleRate);
        else
            exportor = exportFactory.create(format, fileNameW.c_str(), sampleRate);

        if(exportor->isOpened())
            printf(trp("Export to %s\n"), qPrintable(destFileFullName));
        else {
            exportFactory.recycle();
            nFail++;
            printf(trp("Failed to create %s\n"), qPrintable(destFileFullName));
            continue;
        }
        int samples = plugin->getSampleSize(sampleRate, SOUND_FPS);
        if(samples < 2)
            samples = 4096;

        int bufSize = samples*4;
        short *buf = (short*)malloc(bufSize);
        int silentFrames = 0;
        float framesPerSec = sampleRate*4/bufSize;

        int realDuration = duration;
        if(realDuration < 1)
            realDuration = trackInfo.duration;
        if(realDuration < 1)
            realDuration = 0x7fffffff;

        int totalFrames = framesPerSec*((float)realDuration/1000+0.5) + 0.5;

        for(int i=0;i<totalFrames;i++)
        {
            memset(buf, 0, bufSize);
            nextframe(samples, buf);
            if(isBlankFrame(buf, samples))
            {
                silentFrames++;
            } else {
                silentFrames = 0;
            }
            if(silentFrames >= framesPerSec){
                // Frames end
                break;
            }
            exportor->write((const byte*)buf, bufSize);
        }

        nSuccess++;
        exportFactory.recycle();
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

