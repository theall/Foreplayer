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
#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <string>
#include <vector>

using namespace std;

wstring char2wstring(const char *src);
wstring string2wstring(string s);
string wstring2string(wstring ws);
wstring extractSuffix(wstring fileName);
wstring extractPath(wstring fileName);
void readFile(wstring fileName, char **buffer, long *size);
wstring extractBaseName(wstring fileName);
void trim(wstring &ws);
void trim(string &s);
void split(wstring s, wstring delim, vector<wstring > *ret);

void lower(wstring &s);
wstring join(vector<wstring> wsl, wstring delim);

bool contains(vector<wstring> wsl, wstring delim, bool caseSensitive);
const char *extractSuffix(const char *src);
bool contains(vector<wstring> sl, wstring s, bool caseSensitive = false);
bool contains(vector<string> sl, string s, bool caseSensitive = false);

#endif // UTILS_INCLUDED
