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
#include "languagemanager.h"

#include "preferences.h"

#include <QLocale>
#include <QTranslator>
#include <QDirIterator>
#include <QLibraryInfo>
#include <QCoreApplication>

TLanguageManager* TLanguageManager::mInstance = NULL;

TLanguageManager* TLanguageManager::instance()
{
    if (!mInstance)
        mInstance = new TLanguageManager();
    return mInstance;
}

void TLanguageManager::deleteInstance()
{
    if(mInstance)
    {
        delete mInstance;
        mInstance = nullptr;
    }
}

TLanguageManager::TLanguageManager()
    : mQtTranslator(nullptr)
    , mAppTranslator(nullptr)
{
    mTranslationsDir = QCoreApplication::applicationDirPath();
#ifdef Q_OS_WIN32
    mTranslationsDir += QLatin1String("/ts");
#elif defined(Q_OS_MAC)
    mTranslationsDir += QLatin1String("/../ts");
#else
    mTranslationsDir += QLatin1String("/../share/foreplayer/ts");
#endif
}

TLanguageManager::~TLanguageManager()
{
    if(mQtTranslator)
        delete mQtTranslator;
    if(mAppTranslator)
        delete mAppTranslator;
}

void TLanguageManager::installTranslators()
{
    // Delete previous translators
    if(mQtTranslator)
        delete mQtTranslator;
    if(mAppTranslator)
        delete mAppTranslator;

    mQtTranslator = new QTranslator;
    mAppTranslator = new QTranslator;

    QString language = TPreferences::instance()->language();
    if (language.isEmpty())
        language = QLocale::system().name();

    const QString qtTranslationsDir =
            QLibraryInfo::location(QLibraryInfo::TranslationsPath);

    if (mQtTranslator->load(QLatin1String("qt_") + language,
                            qtTranslationsDir)) {
        QCoreApplication::installTranslator(mQtTranslator);
    } else {
        delete mQtTranslator;
        mQtTranslator = nullptr;
    }

    if (mAppTranslator->load(language, mTranslationsDir)) {
        QCoreApplication::installTranslator(mAppTranslator);
    } else {
        delete mAppTranslator;
        mAppTranslator = nullptr;
    }
}

QStringList TLanguageManager::languages()
{
    if (mLanguages.isEmpty())
        loadLanguages();

    return mLanguages;
}

void TLanguageManager::loadLanguages()
{
    mLanguages.clear();

    QStringList nameFilters;
    nameFilters.append(QLatin1String("*.qm"));

    QDirIterator iterator(mTranslationsDir, nameFilters,
                          QDir::Files | QDir::Readable);

    while (iterator.hasNext()) {
        iterator.next();
        const QString baseName = iterator.fileInfo().completeBaseName();
        mLanguages.append(baseName);
    }
}
