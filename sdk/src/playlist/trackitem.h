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

#ifndef TTRACKITEM_H
#define TTRACKITEM_H

#include <string>
#include <vector>

#include <json/json.hpp>

#include "../cplusutils.h"

#include <foreplayer.h>

using namespace std;
using namespace nlohmann;

class TTrackItem
{
public:
    wstring displayName;
    wstring originalName;
    wstring index;
    wstring indexName;
    wstring additionalInfo;
    wstring fileName;
    wstring system;
    wstring artist;
    wstring year;
    int duration;
    int originalDuration;
    bool enable;
    bool hidden;
    TTrackItem();

    ~TTrackItem();

    json toJson();
    void fromJson(json object);

};
typedef vector<TTrackItem*> TTrackItems;

#endif // TTRACKITEM_H
