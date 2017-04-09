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

#ifndef TJSONWRAP_H
#define TJSONWRAP_H


#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <list>
#include <string>

using namespace std;
using namespace rapidjson;

typedef rapidjson::GenericDocument<rapidjson::UTF16<> > DocumentUTF16;
typedef rapidjson::GenericValue<rapidjson::UTF16<> > ValueUTF16;
typedef rapidjson::GenericStringBuffer<rapidjson::UTF16<> > StringBufferUTF16;
typedef rapidjson::Writer<StringBufferUTF16, rapidjson::UTF16<>, rapidjson::UTF16<> > WriterUTF16;

class TJsonWrap
{
public:
    TJsonWrap(wstring filePath);
    ~TJsonWrap();

    ValueUTF16 value(wstring key);

    void setValue(wstring key, const char *value);
    void setValue(wstring key, const wstring value);
    void setValue(wstring key, const int value);
    void setValue(wstring key, const list<wstring> value);

    void save();

private:
    wstring mFileName;
    DocumentUTF16 mDocument;
    DocumentUTF16::AllocatorType mAllocator;
    ValueUTF16 mJsonObject;
};

#endif // TJSONWRAP_H
