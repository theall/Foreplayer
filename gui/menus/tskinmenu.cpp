#include "tskinmenu.h"

TSkinMenu::TSkinMenu(QWidget *parent) :
    TAbstractMenu(parent),
    mLastActivedAction(NULL)
{
    mNameFilter << "*.zip";
    mNameFilter << "*.skn";
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
        skinNames.append(getSkinNameFromZip(file));
        skinFullNames.append(file);
    }

    fileList = dir.entryList(QDir::Dirs|QDir::NoDot|QDir::NoDotDot);
    for(auto file : fileList)
    {
        skinNames.append(getSkinNameFromXml(file+"/skin.xml"));
        skinFullNames.append(file);
    }

    for(int i=0;i<skinNames.count();i++)
    {
        QAction *action = addAction(skinNames[i], this, SLOT(slotSkinTriggered()));
        action->setCheckable(true);
        action->setData(skinFullNames[i]);
    }
}

void TSkinMenu::slotSkinTriggered()
{
    QAction *action = static_cast<QAction*>(sender());

    if(mLastActivedAction)
        mLastActivedAction->setChecked(false);

    mLastActivedAction = action;

    if(action)
        emit requestLoadSkin(action->data().toString());
}

QString TSkinMenu::getSkinNameFromZip(QString zipFile)
{
    return QString();
}

QString TSkinMenu::getSkinNameFromXml(QString xmlFile)
{
    QString name;
    QByteArray data;
    QFile file(xmlFile);
    data.reserve(256);
    int readed = file.read(data.data(), data.size());
    if(readed<1)
        return name;

    name = data;

    QRegExp reg("name *= *\"(.+?)\"", Qt::CaseInsensitive);
    if (reg.indexIn(name) != -1) {
        name = reg.cap(1);
    }

    return name;
}

void TSkinMenu::retranslateUi()
{
}
