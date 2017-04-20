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
#ifndef TSKINSETTING_H
#define TSKINSETTING_H

#include <QSettings>

class TSkinSetting
{
public:
    explicit TSkinSetting(QString skinName);
    ~TSkinSetting();

    void writeMainWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeEqualizerWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeDesktopLyricWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeLyricWindowState(const QByteArray &geometry, const QByteArray &state);
    void writePlaylistWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeBrowserWindowState(const QByteArray &geometry, const QByteArray &state);

    void mainWindowState(QByteArray *geometry, QByteArray *state);
    void equalizerWindowState(QByteArray *geometry, QByteArray *state);
    void desktopLyricWindowState(QByteArray *geometry, QByteArray *state);
    void lyricWindowState(QByteArray *geometry, QByteArray *state);
    void playlistWindowState(QByteArray *geometry, QByteArray *state);
    void browserWindowState(QByteArray *geometry, QByteArray *state);

    bool exists() { return mExists; }

private:
    bool mExists;
    QSettings *mSettings;
};

#endif // TSKINSETTING_H
