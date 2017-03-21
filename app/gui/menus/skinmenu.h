#ifndef TSKINMENU_H
#define TSKINMENU_H

#include "abstractmenu.h"

class TSkinMenu : public TAbstractMenu
{
    Q_OBJECT
public:
    TSkinMenu(QWidget *parent = 0);

signals:
    void requestLoadSkin(int index);
    void requestSkinNames(QStringList &skinNames);

private slots:
    void slotSkinTriggered(bool checked);

private:
    QAction *mLastActivedAction;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
};

#endif // TSKINMENU_H
