#ifndef TPOPMENUFIND_H
#define TPOPMENUFIND_H

#include "abstractpopmenu.h"

class TPopMenuFind : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuFind(QWidget *parent = 0);
    ~TPopMenuFind();

signals:
    void onActionMarkTriggered();
    void onActionFindTriggered();
    void onActionFindNextTriggered();

private:
    QAction *mActionMark;
    QAction *mActionFind;
    QAction *mActionFindNext;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};


#endif // TPOPMENUFIND_H
