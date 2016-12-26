#include "popmenuremovemusics.h"

TPopMenuRemoveMusics::TPopMenuRemoveMusics(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionRemoveSelections = addAction(QString(), this, SIGNAL(onActionRemoveSelectionsTriggered()));
    mActionRemoveRedundant = addAction(QString(), this, SIGNAL(onActionRemoveRedundantTriggered()));
    mActionRemoveErrors = addAction(QString(), this, SIGNAL(onActionRemoveErrorsTriggered()));
    addSeparator();
    mActionRemoveAll = addAction(QString(), this, SIGNAL(onActionRemoveAllTriggered()));
    retranslateUi();
}

TPopMenuRemoveMusics::~TPopMenuRemoveMusics()
{

}

void TPopMenuRemoveMusics::retranslateUi()
{
    setTitle(tr("Remove"));

    mActionRemoveSelections->setText(tr("Selections"));
    mActionRemoveRedundant->setText(tr("Redundant"));
    mActionRemoveErrors->setText(tr("Errors"));
    mActionRemoveAll->setText(tr("All"));
}
