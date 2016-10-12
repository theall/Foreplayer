/*
 * Copyright (C) Bilge Theall
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

#ifndef KNMUSICBACKENFAMILYGAMETHREAD_H
#define KNMUSICBACKENFAMILYGAMETHREAD_H

#include "gme.h"

#include "knmusicutil.h"

#include "knmusicstandardbackendthread.h"

using namespace MusicUtil;

/*!
 * \brief The KNMusicBackendFamilyGameThread class is the standard playing thread for
 * the bass backend. It should be constructed only in the KNMusicBackendBass.
 */
class KNMusicBackendFamilyGameThread : public KNMusicStandardBackendThread
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendFamilyGameThread object.
     * \param parent The KNMusicBackendBass object.
     */
    explicit KNMusicBackendFamilyGameThread(QObject *parent = 0);
    ~KNMusicBackendFamilyGameThread();

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::loadFile().
     */
    bool loadFile(const QString &filePath) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::stop().
     */
    void stop() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::play().
     */
    void play() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::pause().
     */
    void pause() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::volume().
     */
    int volume() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::duration().
     * \return
     */
    qint64 duration() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::position().
     */
    qint64 position() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::state().
     */
    int state() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPlaySection().
     */
    void setPlaySection(const qint64 &start=-1,
                        const qint64 &duration=-1) Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief This signal is used only for threadReachesEnd(). It's used for
     * avoid the 'QObject::killTimer: Timers cannot be stopped from another
     * thread' bug.
     */
    void reachesFinished();

public slots:
    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::save().
     */
    void save() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::restore().
     */
    void restore(const QString &updatedFilePath=QString()) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setVolume().
     */
    void setVolume(const int &volume) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPosition().
     */
    void setPosition(const qint64 &position) Q_DECL_OVERRIDE;

    /*!
     * \brief Set the flags when using at creating a stream.
     * \param channelFlags The new channel flags.
     */
    void setCreateFlags(const DWORD &channelFlags);

    /*!
     * \brief Get track ended inforamtion.
     * \param
     */
    bool trackEnded() const;

    /*!
     * \brief Set stereo depth.
     * \param stereo depth.
     */
    void setStereoDepth( double tempo );

    /*!
     * \brief Enable/disable accuracy.
     * \param bool value.
     */
    void enableAccuracy( bool b );

private slots:
    void checkPosition();

private:
    static void CALLBACK threadReachesEnd(HSYNC handle,
                                          DWORD channel,
                                          DWORD data,
                                          void *user);
    inline void finishPlaying();
    inline void resetChannelDuration()
    {
        //Set the duration to the total duration.
        m_duration=m_totalDuration;
        //Set the start position at the very beginning.
        m_startPosition=0;
        //Set the default end position as the whole file.
        m_endPosition=m_duration;
    }

    inline void setPlayingState(const int &state);
    inline void setChannelSyncs()
    {
        m_syncHandlers.append(BASS_ChannelSetSync(emu_,
                                                  BASS_SYNC_END,
                                                  0,
                                                  threadReachesEnd,
                                                  this));
    }

    inline void removeChannelSyncs();
    inline qint64 getChannelPosition()
    {
        //Get the current position
        return (qint64)
                (BASS_ChannelBytes2Seconds(
                     emu_,
                     BASS_ChannelGetPosition(emu_, BASS_POS_BYTE))
                 //Translate the unit from second to msecond, remove the start
                 //position msecond.
                 *1000)-m_startPosition;
    }
    inline qreal getChannelVolume()
    {
        //Check if the thread is loaded.
        if(!emu_)
        {
            //For a unloaded volume it will be 0.
            return 0.0;
        }
        //Initial a cache.
        float channelVolume;
        //Get the volume to the cache.
        BASS_ChannelGetAttribute(emu_, BASS_ATTRIB_VOL, &channelVolume);
        //Give back the channel volume.
        return channelVolume;
    }
    inline void freeChannel()
    {
        //Check if the channel is not null.
        if(emu_)
        {
            //Free the streams or the music.
            if(!BASS_StreamFree(emu_))
            {
                BASS_MusicFree(emu_);
            }
            //Reset the channel.
            emu_=0;
        }
    }
    inline bool loadBassThread(const QString &filePath)
    {
        //Clear the file path.
        m_filePath.clear();
        //Try to load the file.
    #ifdef Q_OS_WIN
        std::wstring uniPath=filePath.toStdWString();
    #endif
    #ifdef Q_OS_UNIX
        std::string uniPath=filePath.toStdString();
    #endif
        //Create the file using the stream.
        gme_err_t error = gme_open_file( path, &emu_, sample_rate );

        //NULL if no error (success)
        if(error)
        {
            //Emit load failed signal.
            emit loadFailed();
            //Bass is failed to load the music file.
            return false;
        }
        //Save the new file path.
        m_filePath=filePath;

        //Load success.
        return true;
    }

    //Channel data.
    QString m_filePath;
    DWORD emu_;
    DWORD emu_Flags;

    //Channel information.
    qint64 m_totalDuration,
           m_duration,
           m_startPosition,
           m_endPosition;
    qint64 m_savedPosition;
    qreal m_volume;
    int m_state;

    //Updater.
    QTimer *m_positionUpdater;

    Music_Emu* emu_;
    gme_info_t* track_info_;
};

#endif // KNMUSICBACKENFAMILYGAMETHREAD_H
