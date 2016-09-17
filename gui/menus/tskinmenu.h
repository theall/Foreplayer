#ifndef TSKINMENU_H
#define TSKINMENU_H

#include "tabstractmenu.h"

class TSkinMenu : public TAbstractMenu
{
public:
    TSkinMenu(QWidget *parent = 0);

    void setPath(QString skinPath);
    void reload();

signals:
    void requestLoadSkin(QString skinName);

private slots:
    void slotSkinTriggered();

private:
    QString mPath;
    QStringList mNameFilter;
    QAction *mLastActivedAction;
    QString getSkinNameFromZip(QString zipFile);
    QString getSkinNameFromXml(QString xmlFile);

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TSKINMENU_H
