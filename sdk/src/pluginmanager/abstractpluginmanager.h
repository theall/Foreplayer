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

#ifndef ABSTRACTPLUGINMANAGER_H
#define ABSTRACTPLUGINMANAGER_H

#include "backend/backendplugin.h"

#include <vector>
typedef vector<TBackendPlugin*> TBackendPlugins;

/*!
 * \brief The TAbstractPluginManager class is base class of PluginLoader
 */
class TAbstractPluginManager
{
public:
    /*!
     * \brief Construct a TAbstractPluginManager with a parent object.
     * \param parent The parent object of the TAbstractPluginManager.
     */
    explicit TAbstractPluginManager();
    virtual ~TAbstractPluginManager();

    static void setPluginDir(wstring dir);

    /*!
     * \brief Find plugins in specify path.
     * \param path The path of plugins.
     */
    int findPlugins(wstring path);

    virtual TBackendPlugin* loadPlugin(wstring pluginName) = 0;

    inline TBackendPlugins plugins()
    {
        return mPlugins;
    }

protected:
    TBackendPlugin *mDefaultPlugin;
    TBackendPlugins mPlugins;

private:
    static wstring mPluginDir;
};

#endif // ABSTRACTPLUGINMANAGER_H
