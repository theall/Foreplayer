// Game_Music_Emu 0.6.0. http://www.slack.net/~ant/
/* Copyright (C) 2005-2010 by Shay Green. Permission is hereby granted, free of
charge, to any person obtaining a copy of this software module and associated
documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the
following conditions: The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "gmewrap.h"

#include <string.h>
#include <ctype.h>

#define RETURN_ERR( expr ) \
	do {\
		gme_err_t err_ = (expr);\
		if ( err_ )\
            return false;\
	} while ( 0 )

// Number of audio buffers per second. Adjust if you encounter audio skipping.
const int fill_rate = 45;
TGmeWrap *TGmeWrap::mInstance=NULL;

TGmeWrap::TGmeWrap()
{
    mEmulator = 0;
    mScopeBuf = 0;
    mPaused = false;
    mTrackInfo = NULL;
}

bool TGmeWrap::init( long sampleRate )
{
    mSampleRate = sampleRate;
	
    int min_size = mSampleRate * 2 / fill_rate;
	int buf_size = 512;
	while ( buf_size < min_size )
		buf_size *= 2;
	
    return true;
}

void TGmeWrap::free()
{
    gme_delete( mEmulator );
    mEmulator = NULL;
}

TGmeWrap::~TGmeWrap()
{
    free();
    gme_free_info( mTrackInfo );
}

TGmeWrap *TGmeWrap::instance()
{
    if(!mInstance)
        mInstance = new TGmeWrap;

    return mInstance;
}

void TGmeWrap::deleteInstance()
{
    if(mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

bool TGmeWrap::loadFile( const char* path )
{
    free();
	
    RETURN_ERR( gme_open_file( path, &mEmulator, mSampleRate ) );

    return true;
}

bool TGmeWrap::loadData(void *data, int size)
{
    free();

    RETURN_ERR( gme_open_data( data, size, &mEmulator, mSampleRate ) );

    return true;
}

int TGmeWrap::trackCount() const
{
    return mEmulator ? gme_track_count( mEmulator ) : 0;
}

const gme_info_t *TGmeWrap::trackInfo(int index)
{
    if ( mEmulator )
    {
        gme_free_info( mTrackInfo );
        mTrackInfo = NULL;
        gme_err_t error = gme_track_info( mEmulator, &mTrackInfo, index );
        if(error)
            return NULL;
    }

    return mTrackInfo;
}

bool TGmeWrap::startTrack( int track )
{
    if ( mEmulator )
    {
        gme_free_info( mTrackInfo );
        mTrackInfo = NULL;
        RETURN_ERR( gme_track_info( mEmulator, &mTrackInfo, track ) );
	
		// Sound must not be running when operating on emulator
        RETURN_ERR( gme_start_track( mEmulator, track ) );
		
		// Calculate track length
        if ( mTrackInfo->length <= 0 )
            mTrackInfo->length = mTrackInfo->intro_length +
                        mTrackInfo->loop_length * 2;
		
        if ( mTrackInfo->length <= 0 )
            mTrackInfo->length = (long) (2.5 * 60 * 1000);
        gme_set_fade( mEmulator, mTrackInfo->length );
	}
    return true;
}

bool TGmeWrap::seek(int mSeconds)
{
    if(!mEmulator)
        return false;

    gme_err_t error = gme_seek(mEmulator, mSeconds);
    if(error)
        return false;

    return true;
}

bool TGmeWrap::trackEnded() const
{
    return mEmulator ? (bool)gme_track_ended( mEmulator ) : false;
}

void TGmeWrap::setStereoDepth( double tempo )
{
    gme_set_stereo_depth( mEmulator, tempo );
}

void TGmeWrap::enableAccuracy( bool b )
{
    gme_enable_accuracy( mEmulator, b );
}

void TGmeWrap::setTempo( double tempo )
{
    gme_set_tempo( mEmulator, tempo );
}

void TGmeWrap::muteVoices( int mask )
{
    gme_mute_voices( mEmulator, mask );
    gme_ignore_silence( mEmulator, mask != 0 );
}

void TGmeWrap::setScopeBuffer(sample_t *buf, int size)
{
    mScopeBuf = buf;
    mScopeBufSize = size;
}

void TGmeWrap::fillBuffer( sample_t* out, int count )
{
    if ( mEmulator )
	{
        if ( gme_play( mEmulator, count, out ) ) { } // ignore error
        if ( mScopeBuf )
            memcpy( mScopeBuf, out, mScopeBufSize * sizeof *mScopeBuf );
	}
}
