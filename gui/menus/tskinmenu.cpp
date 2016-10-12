#include "tskinmenu.h"

#define BUF_SIZE 256

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
        QString fullName = dir.absoluteFilePath(file);
        QString s = getSkinNameFromZip(fullName);
        if(s.isEmpty())
            continue;

        skinNames.append(s);
        skinFullNames.append(fullName);
    }

    fileList = dir.entryList(QDir::Dirs|QDir::NoDot|QDir::NoDotDot);
    for(auto file : fileList)
    {
        QString fullName = dir.absoluteFilePath(file+"/skin.xml");
        QString s = getSkinNameFromXml(fullName);
        if(s.isEmpty())
            continue;

        skinNames.append(s);
        skinFullNames.append(fullName);
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
    Q_UNUSED(zipFile)

    return QString();
}

QString TSkinMenu::getSkinNameFromXml(QString xmlFile)
{
    QString name;
    QFile file(xmlFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file"<<xmlFile;
        return name;
    }

    char szBuf[BUF_SIZE];
    int readed = file.read(szBuf, BUF_SIZE);
    file.close();
    if(readed<1)
        return name;

    name = szBuf;

    QRegExp reg("<skin +version *=.+ +name *= *\"(.+)\" *author", Qt::CaseInsensitive);
    int pos = reg.indexIn(name);
    if (pos > -1) {
        name = reg.cap(1);
    }

    return name;
}

void TSkinMenu::retranslateUi()
{
}

void TSkinMenu::showEvent(QShowEvent *event)
{
    reload();

    TAbstractMenu::showEvent(event);
}
