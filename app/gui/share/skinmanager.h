#ifndef TSKINMANAGER_H
#define TSKINMANAGER_H

#include <pch.h>
#include "skin.h"

typedef QList<TSkin*> TSkins;
class TSkinManager
{
public:
    TSkinManager();

    void setPath(QString path);
    void reload();
    QStringList skinNames();
    TSkin *skinAt(int i);

private:
    QString mSkinPath;
    TSkins mSkins;
    QStringList mNameFilter;
};
#endif // TSKINMANAGER_H
