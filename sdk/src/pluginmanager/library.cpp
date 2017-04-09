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
#include "library.h"

TLibrary::TLibrary(wstring filePath) :
    mHandle(NULL)
{
#ifdef _WIN32
    mHandle = LoadLibraryW(filePath.c_str());
#else
#endif
}

TLibrary::~TLibrary()
{
#ifdef _WIN32
    if(mHandle)
    {
        FreeLibrary(mHandle);
        mHandle = NULL;
    }
#else
#endif
}

TFunctionPointer TLibrary::resolve(const char *symbol)
{
    TFunctionPointer func;
#ifdef _WIN32
    func = (TFunctionPointer)GetProcAddress(mHandle, symbol);
#else
#endif

    return func;
}
