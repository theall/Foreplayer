#include "popmenutracklist.h"

TPopMenuTrackList::TPopMenuTrackList(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionPlay = new QAction(this);
    mActionCopy = new QAction(this);
    mActionChange = new QAction(this);
    mActionExport = new QAction(this);
    mActionView = new QAction(this);

    connect(mActionPlay, SIGNAL(triggered()), this, SIGNAL(onActionPlayTriggered()));
    connect(mActionCopy, SIGNAL(triggered()), this, SIGNAL(onActionCopyTriggered()));
    connect(mActionChange, SIGNAL(triggered()), this, SIGNAL(onActionChangeTriggered()));
    connect(mActionExport, SIGNAL(triggered()), this, SIGNAL(onActionExportTriggered()));
    connect(mActionView, SIGNAL(triggered()), this, SIGNAL(onActionViewTriggered()));

    addAction(mActionPlay);
    addSeparator();
    addAction(mActionCopy);
    addSeparator();
    addAction(mActionChange);
    addSeparator();
    addAction(mActionExport);
    addSeparator();
    addAction(mActionView);

    retranslateUi();
}

void TPopMenuTrackList::retranslateUi()
{
    mActionPlay->setText(tr("Play"));
    mActionCopy->setText(tr("Copy"));
    mActionChange->setText(tr("Rename/Change"));
    mActionExport->setText(tr("Export"));
    mActionView->setText(tr("View property"));
}
