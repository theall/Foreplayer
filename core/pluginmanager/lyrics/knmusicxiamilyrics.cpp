/*
 * Copyright (C) Kreogist Dev Team
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QLinkedList>
#include <QDomDocument>
#include <QRegularExpression>

#include "knmusicxiamilyrics.h"

#include <QDebug>

KNMusicXiamiLyrics::KNMusicXiamiLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
}

QString KNMusicXiamiLyrics::downloaderName()
{
    return "XiaMi Music";
}

void KNMusicXiamiLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                        QList<KNMusicLyricsDetails> &lyricsList)
{
    //Generate the url.
    QString url="http://www.xiami.com/search/song-lyric?key=" +
                processKeywords(detailInfo.textLists[Name].toString());
    //Get the data from url.
    QByteArray responseData;
    get(url, responseData);
    if(responseData.isEmpty())
    {
        return;
    }
    QString xmlhttpText=responseData;
    //Parse the data.
    QStringList songid;
    QRegularExpression rex("<a.*?href=\".*?/song/(\\d+).*?><b.*?key_red");
    QRegularExpressionMatchIterator i=rex.globalMatch(xmlhttpText);
    while(i.hasNext())
    {
        QRegularExpressionMatch match=i.next();
        songid.append(match.captured(1));
    }
    for(QStringList::iterator i=songid.begin();
        i!=songid.end();
        ++i)
    {
        url="http://www.xiami.com/song/playlist/id/"+(*i);
        get(url, responseData);
        if(!responseData.isEmpty())
        {
            QLinkedList<KNMusicLyricsDetails> rawLyricsDetail;
            QStringList lyricsUrlList;
            //Parse the document.
            QDomDocument lyricsDocument;
            lyricsDocument.setContent(responseData);
            //Get the tracklist.
            QDomElement root=lyricsDocument.documentElement();
            QDomNodeList trackList=root.elementsByTagName("trackList");
            //Get the information from the tracklist.
            for(int j=0; j<trackList.size(); j++)
            {
                QDomElement currentTrack=trackList.at(j).toElement();
                //Find lyrics url.
                QDomNodeList lyricUrlList=
                        currentTrack.elementsByTagName("lyric");
                if(!lyricUrlList.isEmpty())
                {
                    QDomNodeList lyricsUrl=
                            lyricUrlList.at(0).toElement().childNodes();
                    if(!lyricsUrl.isEmpty())
                    {
                        //Save the lyrics information.
                        KNMusicLyricsDetails currentDetail;
                        currentDetail.title=getContentText(&currentTrack,
                                                           "title");
                        currentDetail.artist=getContentText(&currentTrack,
                                                            "artist");
                        //Add the lyrics information and url to the list.
                        rawLyricsDetail.append(currentDetail);
                        lyricsUrlList.append(lyricsUrl.at(0).nodeValue());
                    }
                }
            }

            //Try to download the lyrics.
            for(QStringList::iterator i=lyricsUrlList.begin();
                i!=lyricsUrlList.end();
                ++i)
            {
                KNMusicLyricsDetails currentDetail=rawLyricsDetail.takeFirst();
                //Get the data from the url.
                get((*i), responseData);
                //This is the lyrics file! Add to the lyrics list.
                if(!responseData.isEmpty())
                {
                    saveLyrics(detailInfo,
                               responseData,
                               currentDetail,
                               lyricsList);
                }
            }
        }
    }
}

inline QString KNMusicXiamiLyrics::getContentText(QDomElement *currentTrack,
                                                  const QString &tagName)
{
    //Get the tag name element.
    QDomNodeList &&elementList=currentTrack->elementsByTagName(tagName);
    //Check element list.
    if(elementList.isEmpty())
    {
        //There should be only 1 element in the list.
        return QString();
    }
    //Translate the first one into element, and get the child nodes.
    elementList=elementList.at(0).toElement().childNodes();
    //Check the element list once more.
    if(elementList.isEmpty())
    {
        //There should be only 1 element in the list.
        return QString();
    }
    //Get the element node value.
    return elementList.at(0).nodeValue();
}
