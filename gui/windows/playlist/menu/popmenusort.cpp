#include "popmenusort.h"

TPopMenuSort::TPopMenuSort(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionSortAsName = addAction(QString(), this, SIGNAL(onActionSortAsNameTriggered()));
    mActionSortAsLength = addAction(QString(), this, SIGNAL(onActionSortAsLengthTriggered()));
    mActionSortAsRandom = addAction(QString(), this, SIGNAL(onActionSortAsRandomTriggered()));
    retranslateUi();
}

TPopMenuSort::~TPopMenuSort()
{

}

void TPopMenuSort::retranslateUi()
{
    setTitle(tr("Sort"));

    mActionSortAsName->setText(tr("As name"));
    mActionSortAsLength->setText(tr("As length"));
    mActionSortAsRandom->setText(tr("Random"));
}
