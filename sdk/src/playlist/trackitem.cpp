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

#include "trackitem.h"

#include "playlistdef.h"

TTrackItem::TTrackItem() :
    TAbstractItem()
{
    enable = true;
    hidden = false;
}

TTrackItem::~TTrackItem()
{

}

json TTrackItem::toJson()
{
    json object;
    object[K_NAME] = displayName;
    object[K_INNER_NAME] = fileName;
    object[K_INDEX] = index;
    object[K_INDEX_NAME] = indexName;
    object[K_ADDITIONAL] = additionalInfo;
    object[K_FILE] = fileName;
    object[K_DURATION] = duration;
    object[K_ENABLE] = enable;
    object[K_HIDDEN] = hidden;
    return object;
}

void TTrackItem::fromJson(json object)
{
    displayName = object[K_NAME].get<wstring>();
    fileName = object[K_INNER_NAME].get<wstring>();
    index = object[K_INDEX].get<wstring>();
    indexName = object[K_INDEX_NAME].get<wstring>();
    additionalInfo = object[K_ADDITIONAL].get<wstring>();
    duration = object[K_DURATION];
    enable = object[K_ENABLE];
    hidden = object[K_HIDDEN];
}
