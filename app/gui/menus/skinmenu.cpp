#include "skinmenu.h"
#include "../share/skin.h"

#define BUF_SIZE 256

TSkinMenu::TSkinMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{
    mNameFilter << "*.zip";
}

void TSkinMenu::setPath(QString skinPath)
{
    mPath = skinPath;
}

void TSkinMenu::reload()
{
    clear();

    QStringList skinNames;
    QStringList skinFullNames;
    QDir dir(mPath);
    QStringList fileList = dir.entryList(mNameFilter);
    for(auto file : fileList)
    {
        QString fullName = dir.absoluteFilePath(file);
        QString s = getSkinNameFromFile(fullName);
        if(s.isEmpty())
            continue;

        skinNames.append(s);
        skinFullNames.append(fullName);
    }

    fileList = dir.entryList(QDir::Dirs|QDir::NoDot|QDir::NoDotDot);
    for(auto file : fileList)
    {
        QString fullName = dir.absoluteFilePath(file+"/skin.xml");
        QString s = getSkinNameFromFile(fullName);
        if(s.isEmpty())
            continue;

        skinNames.append(s);
        skinFullNames.append(fullName);
    }

    emit requestSkinNames(skinNames);
    for(int i=0;i<skinNames.count();i++)
    {
        QAction *action = addAction(skinNames[i], this, SLOT(slotSkinTriggered(bool)));
        action->setCheckable(true);
        action->setData(skinFullNames[i]);
    }
}

void TSkinMenu::slotSkinTriggered(bool checked)
{
    QAction *action = qobject_cast<QAction*>(sender());

    if(checked)
    {
        if(mLastActivedAction)
        {
            mLastActivedAction->blockSignals(true);
            mLastActivedAction->setChecked(false);
            mLastActivedAction->blockSignals(false);
        }
        mLastActivedAction = action;

        if(action)
            emit requestLoadSkin(action->data().toString());
    }
}

QString TSkinMenu::getSkinNameFromFile(QString xmlFile)
{
    TSkin skin;
    skin.load(xmlFile);
    return skin.name();
}

void TSkinMenu::retranslateUi()
{
}

void TSkinMenu::showEvent(QShowEvent *event)
{
    reload();

    TAbstractMenu::showEvent(event);
}
