#ifndef TEQUALIZERMENU_H
#define TEQUALIZERMENU_H

#include "abstractmenu.h"

class TEqualizerMenu : public TAbstractMenu
{
    Q_OBJECT

public:
    TEqualizerMenu(QWidget *parent = 0);

    // TAbstractMenu interface

signals:
    void onRockSelected();
    void onMetalSelected();
    void onElectricSelected();
    void onPopSelected();
    void onJazzSelected();
    void onClassicSelected();
    void onCustomizeSelected();

private slots:
    void slotActionTriggered(bool);

private:
    QAction *mActionRock;
    QAction *mActionMetal;
    QAction *mActionElectric;
    QAction *mActionPop;
    QAction *mActionJazz;
    QAction *mActionClassic;
    QAction *mActionCustomize;
    QAction *mLastActivedAction;

public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TEQUALIZERMENU_H
