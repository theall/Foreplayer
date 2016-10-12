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
#include <QTimer>
#include <QUrl>

#include "sdl.h"

#include "audiodriver.h"

#include "knmusicglobal.h"

#include "knmusicbackendfamilygamethread.h"

#include <QDebug>

static void sound_start()
{
    SDL_PauseAudio( false );
}

static void sound_stop()
{
    SDL_PauseAudio( true );

    // be sure audio thread is not active
    SDL_LockAudio();
    SDL_UnlockAudio();
}

static void sound_cleanup()
{
    sound_stop();
    SDL_CloseAudio();
}

KNMusicBackendFamilyGameThread::KNMusicBackendFamilyGameThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_filePath(QString()),
    emu_(0),
    emu_Flags(0),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    m_savedPosition(-1),
    m_volume(1.0),
    m_state(Stopped),
    m_positionUpdater(new QTimer(this)),
    m_syncHandlers(QList<HSYNC>())
{
    //Configure the position updater.
    m_positionUpdater->setInterval(50);
    connect(m_positionUpdater, &QTimer::timeout,
            this, &KNMusicBackendFamilyGameThread::checkPosition);
    //Link the reachesFinished) signal.
    connect(this, &KNMusicBackendFamilyGameThread::reachesFinished,
            this, &KNMusicBackendFamilyGameThread::finishPlaying);
}

KNMusicBackendFamilyGameThread::~KNMusicBackendFamilyGameThread()
{
    //Stop the position updater.
    m_positionUpdater->stop();

    //stop play
    stop();

    //Clean up sound
    sound_cleanup();

    //Free resource
    gme_free_info( track_info_ );
}

bool KNMusicBackendFamilyGameThread::loadFile(const QString &filePath)
{
    //Stop the current thread first.
    stop();
    //Check is the file the current file.
    if(filePath==m_filePath)
    {
        //Reset the current state.
        resetChannelDuration();
        //Update the duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
        //Mission complete.
        return true;
    }
    //Clear up all the previous data.
    reset();
    //Load the file path.
    if(!loadBassThread(filePath))
    {
        //Load the file failed.
        return false;
    }
    //When loading the file complete, load the channel info to the thread.
    //Get the duration of the whole file.
    m_totalDuration=
            BASS_ChannelBytes2Seconds(emu_,
                                      BASS_ChannelGetLength(emu_,
                                                            BASS_POS_BYTE))
            *1000.0;
    //Emit the duration changed signal.
    emit durationChanged(m_totalDuration);
    //Reset the thread information.
    resetChannelDuration();
    //Load complete.
    return true;
}

void KNMusicBackendFamilyGameThread::reset()
{
    //Stop the position updater.
    m_positionUpdater->stop();
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
    //Reset the channel data.
    m_filePath.clear();
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the stream status data.
    resetChannelDuration();
    //Check the current state is stopped or not.
    setPlayingState(Stopped);
}

void KNMusicBackendFamilyGameThread::stop()
{
    //Check:
    // 1. The state is already stopped.
    // 2. The channel is null.
    if(m_state==Stopped || (!emu_))
    {
        return;
    }

    //Stop the sound.
    sound_stop();
    gme_delete( emu_ );
    emu_ = NULL;

    //Stop the position updater.
    m_positionUpdater->stop();
    //Reset the position to the start position.
    setPosition(0);
    //Update the state.
    setPlayingState(Stopped);
    //Emit stopped signal.
    emit stopped();
}

void KNMusicBackendFamilyGameThread::play()
{
    //Check:
    // 1. The state is already playing.
    // 2. The channel is null.
    if(m_state==Playing || (!emu_))
    {
        return;
    }
    //Check the playing state before.
    if(m_state==Stopped)
    {
        //Check out the duration of the song.
        if(m_duration==0)
        {
            //Finished playing, to on action finished.
            finishPlaying();
            //Mission complete.
            return;
        }
        //Reset the position to fit track playing.
        setPosition(0);
        //Set the volume to the last volume, because of the reset, the
        //volume is back to 1.0.
        BASS_ChannelSetAttribute(emu_, BASS_ATTRIB_VOL, m_volume);
    }
    //Start the position updater.
    m_positionUpdater->start();
    //Play the thread.
    gme_free_info( track_info_ );
    track_info_ = NULL;
    gme_err_t error =  gme_track_info( emu_, &track_info_, track );

    if(error)
        return;

    // Sound must not be running when operating on emulator
    sound_stop();

    gme_err_t error =  gme_start_track( emu_, track );
    if(error)
        return;

    // Calculate track length
    if ( track_info_->length <= 0 )
        track_info_->length = track_info_->intro_length +
                    track_info_->loop_length * 2;

    if ( track_info_->length <= 0 )
        track_info_->length = (long) (2.5 * 60 * 1000);

    gme_set_fade( emu_, track_info_->length );

    setPlayingState(Paused);

    sound_start();
    //Update the state.

    setPlayingState(Playing);
}

void KNMusicBackendFamilyGameThread::pause()
{
    //Check:
    // 1. The state is already paused.
    // 2. The channel is null.
    if(m_state==Paused || (!emu_))
    {
        return;
    }
    //Pause the thread.
    sound_stop();
    //Stop the updater.
    m_positionUpdater->stop();
    //Reset the state.
    setPlayingState(Paused);
}

int KNMusicBackendFamilyGameThread::volume()
{
    //Scale the float number.
    return (int)(getChannelVolume()*100.0);
}

qint64 KNMusicBackendFamilyGameThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendFamilyGameThread::position()
{
    return getChannelPosition();
}

int KNMusicBackendFamilyGameThread::state() const
{
    return m_state;
}

void KNMusicBackendFamilyGameThread::setPlaySection(const qint64 &start,
                                              const qint64 &duration)
{
    //Check the validation of the start position.
    // 1. Start is not null(-1).
    // 2. Start is lesser than duration.
    if(start!=-1 && start<m_duration)
    {
        //Save the start position.
        m_startPosition=start;
        //Check the validation of the duration.
        if(duration==-1 || start+duration>m_duration)
        {
            //We will treat the invalid duration when the start is valid to
            //play to the end of the file.
            m_duration-=m_startPosition;
        }
        else
        {
            //Or else, save the new duration.
            m_duration=duration;
        }
        //Update the end position.
        m_endPosition=m_startPosition+m_duration;
        //Emit the new duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
    }
}

void KNMusicBackendFamilyGameThread::save()
{
    //Pause the thread first.
    BASS_ChannelPause(emu_);
    //Stop the updater.
    m_positionUpdater->stop();
    //Save the position of the current thread.
    m_savedPosition=position();
    //Reset the current playing thread, but saved all the other parameter.
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
}

void KNMusicBackendFamilyGameThread::restore(const QString &updatedFilePath)
{
    //Check out the saved position, if it's -1, means it never saved before.
    //Ignore the invalid call.
    if(m_savedPosition==-1)
    {
        return;
    }
    //Check out the updated file path.
    QString restoreFilePath=
            updatedFilePath.isEmpty()?m_filePath:updatedFilePath;
    //Reload the bass thread.
    loadBassThread(restoreFilePath);
    //Reset the postion.
    setPosition(m_savedPosition);
    //Set the volume to the last volume, because of the reset, the
    //volume is back to 1.0.
    BASS_ChannelSetAttribute(emu_, BASS_ATTRIB_VOL, m_volume);
    //Check out the state.
    if(m_state==Playing)
    {
        //Start the updater.
        m_positionUpdater->start();
        //Play the thread.
        BASS_ChannelPlay(emu_, FALSE);
    }
    //Reset the saved position.
    m_savedPosition=-1;
}

void KNMusicBackendFamilyGameThread::setVolume(const int &volume)
{
    //Check the channel is null.
    if(!emu_)
    {
        return;
    }
    //Set the volume to channel.
    BASS_ChannelSetAttribute(emu_, BASS_ATTRIB_VOL, ((float)volume)/100.0);
    //Save the latest volume size.
    m_volume=getChannelVolume();
}

void KNMusicBackendFamilyGameThread::setPosition(const qint64 &position)
{
    //Check the channel is null.
    if(!emu_)
    {
        return;
    }
    //If the state is stopped and the position is not 0, then changed the state
    //to pause.
    if(m_state==Stopped && position!=0)
    {
        //Change to paused.
        setPlayingState(Paused);
    }
    //Change the position, the unit of the position should be translate into
    //second.
    BASS_ChannelSetPosition(
                emu_,
                BASS_ChannelSeconds2Bytes(emu_,
                                          //The position here should be the
                                          //'absolute' position.
                                          //That means it should be the
                                          //position plus the start
                                          //position.
                                          (double)(m_startPosition+position)
                                          /1000.0),
                BASS_POS_BYTE);
    //Check the position.
    checkPosition();
}

void KNMusicBackendFamilyGameThread::setCreateFlags(const DWORD &channelFlags)
{
    //Save the channel flags.
    emu_Flags=channelFlags;
}

bool KNMusicBackendFamilyGameThread::trackEnded() const
{
    return emu_ ? gme_track_ended( emu_ ) : false;
}

void KNMusicBackendFamilyGameThread::setStereoDepth(double tempo)
{
    sound_stop();

    gme_set_stereo_depth( emu_, tempo );

    sound_start();
}

void KNMusicBackendFamilyGameThread::enableAccuracy(bool b)
{
    sound_stop();

    gme_enable_accuracy( emu_, b );

    sound_start();
}

void KNMusicBackendFamilyGameThread::checkPosition()
{
    //Get the current position.
    qint64 currentPosition=getChannelPosition();
    //Emit position changed signal.
    emit positionChanged(currentPosition);
    //Check the position is longer than the duration.
    /*
     * Q: Why we still need to do this?
     * A: When cue is playing, it may not stopped at the end of the file.
     *    The callback is only used to solve the position won't reach the end
     *    bug, the track duration stopped will still process here.
     */
    if(currentPosition>=m_duration)
    {
        //Finished the playing.
        finishPlaying();
    }
}

void KNMusicBackendFamilyGameThread::threadReachesEnd(HSYNC handle,
                                                DWORD channel,
                                                DWORD data,
                                                void *user)
{
    Q_UNUSED(handle)
    Q_UNUSED(channel)
    Q_UNUSED(data)
    //Finished the playing of the thread.
    //Call the signal function to emit this signal.
    static_cast<KNMusicBackendFamilyGameThread *>(user)->reachesFinished();
}

inline void KNMusicBackendFamilyGameThread::finishPlaying()
{
    //Stop playing.
    stop();
    //Emit finished signal.
    emit finished();
}

inline void KNMusicBackendFamilyGameThread::setPlayingState(const int &state)
{
    //Only process the calling when the state is different.
    if(state!=m_state)
    {
        //Save the new state.
        m_state=state;
        //Emit state changed signal.
        emit stateChanged(m_state);
    }
}

inline void KNMusicBackendFamilyGameThread::removeChannelSyncs()
{
    //Get all the handlers.
    for(auto i=m_syncHandlers.begin(); i!=m_syncHandlers.end(); ++i)
    {
        //Remove all the sync from the list.
        BASS_ChannelRemoveSync(emu_, *i);
    }
    //Clear the hanlder list.
    m_syncHandlers.clear();
}
