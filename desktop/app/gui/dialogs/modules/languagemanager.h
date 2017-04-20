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
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include "pch.h"

class TLanguageManager
{
public:
    static TLanguageManager* instance();
    static void deleteInstance();

    /**
     * Installs the translators on the application for Qt and Tiled. Should be
     * called again when the language changes.
     */
    void installTranslators();

    /**
     * Returns the available languages as a list of country codes.
     */
    QStringList languages();

private:
    TLanguageManager();
    ~TLanguageManager();

    void loadLanguages();

    QString mTranslationsDir;
    QStringList mLanguages;
    QTranslator *mQtTranslator;
    QTranslator *mAppTranslator;

    static TLanguageManager *mInstance;
};

#endif // LANGUAGEMANAGER_H
