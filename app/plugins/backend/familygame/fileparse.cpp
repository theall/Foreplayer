#include "fileparse.h"
#include <QString>

TFileParse::TFileParse(QString file) :
    TAbstractParse(file)
{

}

bool TFileParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    TGmeWrap gme;
    wstring wpath = mFile.toStdWString();
    gme.loadFile(wpath.c_str());
    int trackCount = gme.trackCount();
    int trackIndex = 0;
    for(int i=0;i<trackCount;i++) {
        TTrackInfo *trackInfo = new TTrackInfo;
        const gme_info_t *_trackInfo = gme.trackInfo(i);
        trackInfo->trackName = _trackInfo->song;
        if(trackInfo->trackName=="")
            trackInfo->trackName = "unknown";
        trackInfo->additionalInfo = fillAdditionalInfo(_trackInfo);
        trackInfo->duration  = _trackInfo->length;
//        if(trackInfo->duration <= 0)
//            trackInfo->duration = _trackInfo->play_length;
        trackInfo->index     = trackIndex;
        musicInfo->trackList.push_back(trackInfo);
        musicInfo->duration += trackInfo->duration;
        trackIndex++;
    }
    if(trackCount > 0)
    {
        TTrackInfo *trackInfo = musicInfo->trackList.at(0);
        musicInfo->additionalInfo = trackInfo->additionalInfo;
        if(trackCount==1)
            musicInfo->musicName = trackInfo->trackName;
    }

    return true;
}

TTrackInfo *TFileParse::parse(QByteArray data)
{
    TGmeWrap gme;
    gme.loadData((void*)data.constData(), data.size());
    int trackCount = gme.trackCount();
    if(trackCount != 1)
        return NULL;

    TTrackInfo *trackInfo = new TTrackInfo;
    const gme_info_t *_trackInfo = gme.trackInfo(0);
    trackInfo->trackName = _trackInfo->song;
    trackInfo->index = 0;
    trackInfo->duration  = _trackInfo->length;
    if(trackInfo->duration <= 0)
        trackInfo->duration = _trackInfo->play_length;
    trackInfo->fileSize  = data.size();
    trackInfo->additionalInfo = fillAdditionalInfo(_trackInfo);

    return trackInfo;
}

string TFileParse::fillAdditionalInfo(const gme_info_t *trackInfo)
{
    return QString::asprintf("game: %s\n"
                    "system: %s\n"
                    "author: %s\n"
                    "dumper: %s\n"
                    "copyright: %s\n"
                    "comment: %s",
                    trackInfo->game,
                    trackInfo->system,
                    trackInfo->author,
                    trackInfo->dumper,
                    trackInfo->copyright,
                    trackInfo->comment
                    ).toStdString();
}
