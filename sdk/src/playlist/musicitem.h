/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef TMUSICITEM_H
#define TMUSICITEM_H

#include "trackitem.h"
#include "time.h"

class TMusicItem : public TAbstractItem
{
public:
    TMusicItem();
    virtual ~TMusicItem();

    json toJson();
    void fromJson(json object);

    void fromTrackItem(TTrackItem *trackItem);

    void sort(SortMethod mode);

    wstring displayName();
    bool setDisplayName(wstring displayName);

    wstring originalName();
    void setOriginalName(wstring name);

    wstring fileName();
    void setFileName(wstring fileName);

    wstring system();
    void setSystem(wstring system);

    wstring artist();
    void setArtist(wstring artist);

    wstring year();
    void setYear(wstring year);

    wstring additionalInfo();
    void setAdditionalInfo(wstring info);

    int duration();
    void setDuration(int duration);

    int fileSize();
    void setFileSize(int fileSize);

    wstring game();
    void setGame(wstring game);

    wstring album();
    void setAlbum(wstring album);

    time_t lastParsed();
    void setLastParsed(time_t lastParsed);

    TTrackItems *trackItems();
    TTrackItem *trackItem(int index);
    int indexOf(TTrackItem *trackItem);
    void addTrackItem(TTrackItem *trackItem, int pos=-1);

    bool isModified();
    void setModified(bool modified=true);

    int size();
    void clear();

private:
    wstring mDisplayName;
    wstring mOriginalName;
    wstring mFileName;
    wstring mGame;
    wstring mSystem;
    wstring mArtist;
    wstring mYear;
    wstring mAdditionalInfo;
    int mDuration;
    int mFileSize;
    bool mModified;
    time_t mLastParsed;
    TTrackItems mTrackItems;
};

typedef vector<TMusicItem*> TMusicItems;

#endif // TMUSICITEM_H
