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
#include "skinsetting.h"
#include <QFileInfo>

#define SECTION_WINDOW              "window"
#define KEY_MAIN_GEOMETRY           "main_geometry"
#define KEY_MAIN_STATE              "main_STATE"
#define KEY_EQUALIZER_GEOMETRY      "equalizer_geometry"
#define KEY_EQUALIZER_STATE         "equalizer_STATE"
#define KEY_DESKTOPLYRIC_GEOMETRY   "desktoplyric_geometry"
#define KEY_DESKTOPLYRIC_STATE      "desktoplyric_STATE"
#define KEY_LYRIC_GEOMETRY          "lyric_geometry"
#define KEY_LYRIC_STATE             "lyric_STATE"
#define KEY_PLAYLIST_GEOMETRY       "playlist_geometry"
#define KEY_PLAYLIST_STATE          "playlist_STATE"
#define KEY_BROWSER_GEOMETRY        "browser_geometry"
#define KEY_BROWSER_STATE           "browser_STATE"

TSkinSetting::TSkinSetting(QString skinName)
{
    QFileInfo fi(skinName);
    QDir skinDir = fi.dir();
    QString iniFile = skinDir.absoluteFilePath(fi.baseName()+".ini");
    mExists = QFileInfo(iniFile).exists();
    mSettings = new QSettings(iniFile, QSettings::IniFormat);
    mSettings->beginGroup(SECTION_WINDOW);
}

TSkinSetting::~TSkinSetting()
{
    if(mSettings)
    {
        mSettings->endGroup();
        delete mSettings;
        mSettings = NULL;
    }
}

void TSkinSetting::writeMainWindowState(const QByteArray &geometry, const QByteArray &state)
{
    mSettings->setValue(KEY_MAIN_GEOMETRY, geometry);
    mSettings->setValue(KEY_MAIN_STATE, state);
}

void TSkinSetting::writeEqualizerWindowState(const QByteArray &geometry, const QByteArray &state)
{
    mSettings->setValue(KEY_EQUALIZER_GEOMETRY, geometry);
    mSettings->setValue(KEY_EQUALIZER_STATE, state);
}

void TSkinSetting::writeDesktopLyricWindowState(const QByteArray &geometry, const QByteArray &state)
{
    mSettings->setValue(KEY_DESKTOPLYRIC_GEOMETRY, geometry);
    mSettings->setValue(KEY_DESKTOPLYRIC_STATE, state);
}

void TSkinSetting::writeLyricWindowState(const QByteArray &geometry, const QByteArray &state)
{
    mSettings->setValue(KEY_LYRIC_GEOMETRY, geometry);
    mSettings->setValue(KEY_LYRIC_STATE, state);
}

void TSkinSetting::writePlaylistWindowState(const QByteArray &geometry, const QByteArray &state)
{
    mSettings->setValue(KEY_PLAYLIST_GEOMETRY, geometry);
    mSettings->setValue(KEY_PLAYLIST_STATE, state);
}

void TSkinSetting::writeBrowserWindowState(const QByteArray &geometry, const QByteArray &state)
{
    mSettings->setValue(KEY_BROWSER_GEOMETRY, geometry);
    mSettings->setValue(KEY_BROWSER_STATE, state);
}

void TSkinSetting::mainWindowState(QByteArray *geometry, QByteArray *state)
{
    *geometry = mSettings->value(KEY_MAIN_GEOMETRY).toByteArray();
    *state = mSettings->value(KEY_MAIN_STATE).toByteArray();
}

void TSkinSetting::equalizerWindowState(QByteArray *geometry, QByteArray *state)
{
    *geometry = mSettings->value(KEY_EQUALIZER_GEOMETRY).toByteArray();
    *state = mSettings->value(KEY_EQUALIZER_STATE).toByteArray();
}

void TSkinSetting::desktopLyricWindowState(QByteArray *geometry, QByteArray *state)
{
    *geometry = mSettings->value(KEY_DESKTOPLYRIC_GEOMETRY).toByteArray();
    *state = mSettings->value(KEY_DESKTOPLYRIC_STATE).toByteArray();
}

void TSkinSetting::lyricWindowState(QByteArray *geometry, QByteArray *state)
{
    *geometry = mSettings->value(KEY_LYRIC_GEOMETRY).toByteArray();
    *state = mSettings->value(KEY_LYRIC_STATE).toByteArray();
}

void TSkinSetting::playlistWindowState(QByteArray *geometry, QByteArray *state)
{
    *geometry = mSettings->value(KEY_PLAYLIST_GEOMETRY).toByteArray();
    *state = mSettings->value(KEY_PLAYLIST_STATE).toByteArray();
}

void TSkinSetting::browserWindowState(QByteArray *geometry, QByteArray *state)
{
    *geometry = mSettings->value(KEY_BROWSER_GEOMETRY).toByteArray();
    *state = mSettings->value(KEY_BROWSER_STATE).toByteArray();
}
