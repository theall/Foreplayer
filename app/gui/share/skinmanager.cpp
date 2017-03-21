#include "skinmanager.h"
#include "utils.h"
TSkinManager::TSkinManager()
{
    mSkinPath = Utils::absoluteFilePath("skins");

    mNameFilter << "*.zip";
}

void TSkinManager::setPath(QString path)
{
    mSkinPath = Utils::absoluteFilePath(path);
    reload();
}

void TSkinManager::reload()
{
    mSkins.clear();

    QStringList skinFullNames;
    QDir dir(mSkinPath);
    QStringList fileList = dir.entryList(mNameFilter);
    for(auto file : fileList)
        skinFullNames.append(dir.absoluteFilePath(file));

    fileList = dir.entryList(QDir::Dirs|QDir::NoDot|QDir::NoDotDot);
    for(auto file : fileList)
        skinFullNames.append(dir.absoluteFilePath(file+"/skin.xml"));

    for(auto fileName : skinFullNames)
    {
        TSkin *skin = new TSkin;
        skin->load(fileName);
        if(skin->name().isEmpty())
        {
            delete skin;
            continue;
        }
        mSkins.append(skin);
    }
}

QStringList TSkinManager::skinNames()
{
    QStringList names;
    for(TSkin *skin : mSkins)
    {
        names.append(skin->name());
    }
    return names;
}

TSkin *TSkinManager::skinAt(int i)
{
    if(i>=0 && i<mSkins.size())
        return mSkins[i];
    return NULL;
}

