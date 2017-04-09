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

#include "jsonwrap.h"

#include "../cplusutils.h"

TJsonWrap::TJsonWrap(wstring filePath) :
    mJsonObject(kObjectType)
{
    mAllocator = mDocument.GetAllocator();
    wstring text = readText(filePath);
    mDocument.Parse(text.c_str());
    mFileName = filePath;
}

TJsonWrap::~TJsonWrap()
{
}

ValueUTF16 TJsonWrap::value(wstring key)
{
    return mDocument[key.c_str()];
}

void TJsonWrap::setValue(wstring key, const char *value)
{
    mDocument[key] = value;
}

void TJsonWrap::setValue(wstring key, const wstring value)
{
    mDocument[key] = value;
}

void TJsonWrap::setValue(wstring key, const int value)
{
    mDocument[key] = value;
}

void TJsonWrap::setValue(wstring key, const list<wstring> value)
{

}

void TJsonWrap::save()
{
    FILE *fp = _wfopen(mFileName.c_str(), L"wb");

    fclose(fp);
}

