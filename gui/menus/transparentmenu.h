#ifndef TTRANSPARENTMENU_H
#define TTRANSPARENTMENU_H

#include "abstractmenu.h"

class TTransparentMenu : public TAbstractMenu
{
    Q_OBJECT
public:
    TTransparentMenu(QWidget *parent = 0);

signals:
    void onOpacityChanged(qreal);

private slots:
    void slotActionTriggered(bool);

private:
    QAction *mLastActivedAction;
    QAction *mActionDisable;
    QAction *mActionTransparent1;
    QAction *mActionTransparent2;
    QAction *mActionTransparent3;
    QAction *mActionTransparent4;
    QAction *mActionTransparent5;
    QAction *mActionTransparent6;
    QAction *mActionTransparent7;
    QAction *mActionTransparent8;
    QAction *mActionTransparent9;
    QAction *mActionTransparent10;
    QAction *mActionDisableWhileActived;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TTRANSPARENTMENU_H