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
#ifndef __STR_STR_HPP_INCLUDED_
#define __STR_STR_HPP_INCLUDED_
#include <stdarg.h>
#include <string>
#include <vector>
#include <cassert>

namespace str
{
    std::wstring format(const char *fmt, )
    {
        std::wstring strResult="";
        if (NULL != fmt)
        {
            va_list marker = NULL;
            va_start(marker, fmt);
            size_t nLength = _vscprintf(fmt, marker) + 1;
            std::vector<char> vBuffer(nLength, '\0');
            int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
            if (nWritten>0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker);
        }
        return strResult;
    }

    std::wstring format(const wchar_t *fmt, )
    {
        std::wstring strResult=L"";
        if (NULL != fmt)
        {
            va_list marker = NULL;
            va_start(marker, fmt);
            size_t nLength = _vscwprintf(fmt, marker) + 1;
            std::vector<wchar_t> vBuffer(nLength, L'\0');
            int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
            if (nWritten > 0)
            {
                strResult = &vBuffer[0];
            }
            va_end(marker);
        }
        return strResult;
    }
}
#endif
