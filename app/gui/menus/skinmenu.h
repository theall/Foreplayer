#ifndef TSKINMENU_H
#define TSKINMENU_H

#include "abstractmenu.h"

class TSkinMenu : public TAbstractMenu
{
    Q_OBJECT
public:
    TSkinMenu(QWidget *parent = 0);

    void setPath(QString skinPath);
    void reload();

signals:
    void requestLoadSkin(QString skinName);
    void requestSkinNames(QStringList &skinNames);

private slots:
    void slotSkinTriggered(bool checked);

private:
    QString mPath;
    QStringList mNameFilter;
    QAction *mLastActivedAction;
    QString getSkinNameFromFile(QString xmlFile);

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
};

#endif // TSKINMENU_H
