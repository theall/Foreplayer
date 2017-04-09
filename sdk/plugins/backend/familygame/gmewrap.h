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

#ifndef GAME_WRAP
#define GAME_WRAP

#include <gme.h>

typedef short sample_t;

class TGmeWrap
{
public:
    TGmeWrap(int sampleRate = 44100);
    ~TGmeWrap();

    static TGmeWrap *instance();
    static void deleteInstance();

	// Load game music file. NULL on success, otherwise error wstring.
    bool loadFile( const wchar_t* path );

    // Load data from rsn file
    bool loadData(void *data, int size);

	// (Re)start playing track. Tracks are numbered from 0 to track_count() - 1.
    bool startTrack( int track );

    // Seek
    bool seek(int mSeconds);

    // Fill samples into buffer
    void fillBuffer( sample_t*, int );

    // Free  resource
    void free();

// Optional functions

	// Number of tracks in current file, or 0 if no file loaded.
    int trackCount() const;

	// Info for current track
    const gme_info_t *trackInfo(int trackIndex);

	// True if track ended
    bool trackEnded() const;

	// Set stereo depth, where 0.0 = none and 1.0 = maximum
    void setStereoDepth( double );

	// Enable accurate sound emulation
    void enableAccuracy( bool );

	// Set tempo, where 0.5 = half speed, 1.0 = normal, 2.0 = double speed
    void setTempo( double );

	// Set voice muting bitmask
    void muteVoices( int );

	// Set buffer to copy samples from each buffer into, or NULL to disable
    void setScopeBuffer(sample_t* buf, int size );

private:
    Music_Emu* mEmulator;
    sample_t* mScopeBuf;
    long mSampleRate;
    int mScopeBufSize;
    gme_info_t* mTrackInfo;

    static TGmeWrap *mInstance;
};

#endif
