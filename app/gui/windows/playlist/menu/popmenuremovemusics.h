#ifndef TPOPMENUREMOVEMUSICS_H
#define TPOPMENUREMOVEMUSICS_H

#include "abstractpopmenu.h"

class TPopMenuRemoveMusics : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuRemoveMusics(QWidget *parent = 0);
    ~TPopMenuRemoveMusics();

signals:
    void onActionRemoveSelectionsTriggered();
    void onActionRemoveRedundantTriggered();
    void onActionRemoveErrorsTriggered();
    void onActionRemoveAllTriggered();

private:
    QAction *mActionRemoveSelections;
    QAction *mActionRemoveRedundant;
    QAction *mActionRemoveErrors;
    QAction *mActionRemoveAll;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUREMOVEMUSICS_H
