#include "popmenuaddmusics.h"

TPopMenuAddMusics::TPopMenuAddMusics(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionAddMusics = addAction(QString(), this, SIGNAL(onActionAddMusicsTriggered()));
    mActionAddDirectory = addAction(QString(), this, SIGNAL(onActionAddDirectoryTriggered()));
    retranslateUi();
}

TPopMenuAddMusics::~TPopMenuAddMusics()
{

}

void TPopMenuAddMusics::retranslateUi()
{
    setTitle(tr("Add"));

    mActionAddMusics->setText(tr("Add musics"));
    mActionAddDirectory->setText(tr("Add music directory"));
}
