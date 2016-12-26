#ifndef TPOPMENUSORT_H
#define TPOPMENUSORT_H

#include "abstractpopmenu.h"

class TPopMenuSort : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuSort(QWidget *parent = 0);
    ~TPopMenuSort();

signals:
    void onActionSortAsNameTriggered();
    void onActionSortAsLengthTriggered();
    void onActionSortAsRandomTriggered();

private:
    QAction *mActionSortAsName;
    QAction *mActionSortAsLength;
    QAction *mActionSortAsRandom;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUSORT_H
