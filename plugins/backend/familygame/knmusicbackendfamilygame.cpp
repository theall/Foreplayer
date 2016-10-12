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
#include <QApplication>
#include <QDir>

#include "knglobal.h"

#include "knmusicbackendfamilygamethread.h"

#include "knmusicbackendfamilygame.h"

#include <QDebug>

KNMusicBackendFamilyGame::KNMusicBackendFamilyGame(QObject *parent) :
    KNMusicStandardBackend(parent)
{
    //Initial a empty thread flags.
    DWORD threadFlag=0;
    //Initial the bass libary.
    if(!initialGme(threadFlag))
    {
        return;
    }
    //Load plugins from application resource dir and global plugin dir.
#ifdef Q_OS_MACX
    //This is a special OS, on Mac OS X, all the bass related dylib should be
    //placed at the same path of the executable file.
    loadPlugin(qApp->applicationDirPath());
#else
    //Load the user plugins.
    loadPlugin(knGlobal->dirPath(KNGlobal::PluginDir) + "/Bass");
    //Load the plugins in the application folder.
    loadPlugin(knGlobal->dirPath(KNGlobal::ResourceDir) + "/Plugins/Bass");
#endif
    //Initial the main and preview thread.
    setMainThread(generateThread(threadFlag));
    setPreviewThread(generateThread(threadFlag));
}

KNMusicBackendFamilyGame::~KNMusicBackendFamilyGame()
{
    //Free all the plugin to recover the memory.
    while(!m_pluginList.isEmpty())
    {
        BASS_PluginFree(m_pluginList.takeFirst());
    }
    //Close the bass.
    BASS_Free();
}

int KNMusicBackendFamilyGame::volume() const
{
    return BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
}

int KNMusicBackendFamilyGame::minimalVolume() const
{
    return 0;
}

int KNMusicBackendFamilyGame::maximumVolume() const
{
    return 10000;
}

void KNMusicBackendFamilyGame::setGlobalVolume(const int &volume)
{
    //Change the global volume size of the bass.
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume);
    //Emit the volume changed signal.
    emit volumeChanged(volume);
}

int KNMusicBackendFamilyGame::volumeLevel() const
{
    return 1000;
}

qreal KNMusicBackendFamilyGame::smartVolumeScale() const
{
    return 0.125;
}

inline bool KNMusicBackendFamilyGame::initialGme(long rate)
{
    sample_rate = rate;

    int min_size = sample_rate * 2 / fill_rate;
    int buf_size = 512;
    while ( buf_size < min_size )
        buf_size *= 2;

    return sound_init( sample_rate, buf_size, fill_buffer, this );
}

inline void KNMusicBackendFamilyGame::loadPlugin(const QString &dirPath)
{
    //Get the dynamic linked library suffix name.
    QString dylibSuffix;
#ifdef Q_OS_WIN
    dylibSuffix=QString("dll");
#endif
#ifdef Q_OS_MACX
    dylibSuffix=QString("dylib");
#endif
#ifdef Q_OS_LINUX
    dylibSuffix=QString("so");
#endif
    //Check if we have already get the suffix for current OS.
    if(dylibSuffix.isEmpty())
    {
        //Ignore the load request if we cannot get the suffix.
        return;
    }
    //Get the plugin directory.
    QDir pluginDir(dirPath);
    //Check the diretory existance.
    if(!pluginDir.exists())
    {
        return;
    }
    //Get all the file from the plugin dir.
    QFileInfoList pluginList=pluginDir.entryInfoList();
    //Check all the file.
    for(auto i=pluginList.constBegin(); i!=pluginList.constEnd(); ++i)
    {
        //If the item is file, and the suffix is dynamic linked library, then
        //we have to tried to load the plugin.
        if((*i).isFile() && (*i).suffix().toLower()==dylibSuffix)
        {
            //Load the plugin.
            HPLUGIN plugin=
            //For windows, we have to use stdWString.
#ifdef Q_OS_WIN
            BASS_PluginLoad((*i).absoluteFilePath().toStdWString().data(), 0);
#endif
#ifdef Q_OS_UNIX
            BASS_PluginLoad((*i).absoluteFilePath().toStdString().data(), 0);
#endif
            //Add the plugin to the plugin list.
            m_pluginList.append(plugin);
        }
    }
}

KNMusicBackendFamilyGameThread *KNMusicBackendFamilyGame::generateThread(
        const DWORD &channelFlags)
{
    //Generate a thread.
    KNMusicBackendFamilyGameThread *thread=new KNMusicBackendFamilyGameThread(this);
    //Set the channel create flag to the thread.
    thread->setCreateFlags(channelFlags);
    //Give back the thread.
    return thread;
}
