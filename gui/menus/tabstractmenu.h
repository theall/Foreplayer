#ifndef TABSTRACTMENU_H
#define TABSTRACTMENU_H

#include "pch.h"

class TAbstractMenu : public QMenu
{
    Q_OBJECT

public:
    TAbstractMenu(QWidget *parent = 0);
    ~TAbstractMenu();

    virtual void retranslateUi() = 0;

    // QObject interface
public:
    bool event(QEvent *) Q_DECL_OVERRIDE;
};

#endif // TABSTRACTMENU_H
