/*
 * Copyright (C) Kreogist Dev Team
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICBACKENDBASS_H
#define KNMUSICBACKENDBASS_H

/*
 * * August 14th, 2016:
 * This module is the backend of playing family game music, it currently
 * support formats below:
 * AY        ZX Spectrum/Amstrad CPC
 * GBS       Nintendo Game Boy
 * GYM       Sega Genesis/Mega Drive
 * HES       NEC TurboGrafx-16/PC Engine
 * KSS       MSX Home Computer/other Z80 systems (doesn't support FM sound)
 * NSF/NSFE  Nintendo NES/Famicom (with VRC 6, Namco 106, and FME-7 sound)
 * SAP       Atari systems using POKEY sound chip
 * SPC       Super Nintendo/Super Famicom
 * VGM/VGZ   Sega Master System/Mark III, Sega Genesis/Mega Drive,BBC Micro
 *
 * Visit https://bitbucket.org/mpyne/game-music-emu for further information.
 */
#include "gme.h"

#include "knmusicstandardbackend.h"

class KNMusicBackendFamilyGameThread;
/*!
 * \brief The KNMusicBackendFamilyGame class provides you a backend interface of the
 * backend family game music.
 */
class KNMusicBackendFamilyGame : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendBassclass with given parent.
     * \param parent The parent object.
     */
    explicit KNMusicBackendFamilyGame(QObject *parent = 0);
    ~KNMusicBackendFamilyGame();

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::volume().
     */
    int volume() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::minimalVolume().
     */
    int minimalVolume() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::maximumVolume().
     */
    int maximumVolume() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMusicStandardBackend::setGlobalVolume().
     */
    void setGlobalVolume(const int &volume) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::volumeLevel().
     */
    int volumeLevel() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::smartVolumeScale().
     */
    qreal smartVolumeScale() const Q_DECL_OVERRIDE;

private:
    inline bool initialGme(long sample_rate = 44100);
    inline void loadPlugin(const QString &dirPath);
    inline KNMusicBackendFamilyGameThread *generateThread(const DWORD &channelFlags);

    long sample_rate;
};

#endif // KNMUSICBACKENDBASS_H
