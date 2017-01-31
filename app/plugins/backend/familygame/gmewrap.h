// Simple game music file player

// Game_Music_Emu 0.6.0
#ifndef GAME_WRAP
#define GAME_WRAP

#include "gme.h"

typedef short sample_t;

class TGmeWrap
{
public:
    TGmeWrap();
    ~TGmeWrap();

    static TGmeWrap *instance();
    static void deleteInstance();

	// Initialize player and set sample rate
    bool init( long sampleRate = 44100 );
	
	// Load game music file. NULL on success, otherwise error string.
    bool loadFile( const char* path );
	
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
    bool mPaused;
    gme_info_t* mTrackInfo;

    static TGmeWrap *mInstance;
};

#endif
