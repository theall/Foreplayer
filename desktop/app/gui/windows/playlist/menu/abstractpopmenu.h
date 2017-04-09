#ifndef TABSTRACTPOPMENU_H
#define TABSTRACTPOPMENU_H

#include "../../widgets/animationmenu.h"

class TAbstractPopMenu : public QMenu
{
    Q_OBJECT

public:
    TAbstractPopMenu(QWidget *parent = 0);
    ~TAbstractPopMenu();

    virtual void retranslateUi() = 0;

    // QObject interface
public:
    bool event(QEvent *) Q_DECL_OVERRIDE;
};

#endif // TABSTRACTPOPMENU_H
