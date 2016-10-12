#ifndef TEQUALIZERMENU_H
#define TEQUALIZERMENU_H

#include "tabstractmenu.h"

class TEqualizerMenu : public TAbstractMenu
{
    Q_OBJECT

public:
    TEqualizerMenu(QWidget *parent = 0);

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TEQUALIZERMENU_H
