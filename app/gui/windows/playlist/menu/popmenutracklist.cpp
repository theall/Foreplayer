#include "popmenutracklist.h"

TPopMenuTrackList::TPopMenuTrackList(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionPlay = new QAction(this);
    mActionRenameTitle = new QAction(this);
    mActionRenameDuration = new QAction(this);
    mActionExport = new QAction(this);
    mActionView = new QAction(this);

    connect(mActionPlay, SIGNAL(triggered()), this, SIGNAL(onActionPlayTriggered()));
    connect(mActionRenameTitle, SIGNAL(triggered()), this, SIGNAL(onActionRenameTitleTriggered()));
    connect(mActionRenameDuration, SIGNAL(triggered()), this, SIGNAL(onActionRenameDurationTriggered()));
    connect(mActionExport, SIGNAL(triggered()), this, SIGNAL(onActionExportTriggered()));
    connect(mActionView, SIGNAL(triggered()), this, SIGNAL(onActionViewTriggered()));

    addAction(mActionPlay);
    addSeparator();
    addAction(mActionRenameTitle);
    addAction(mActionRenameDuration);
    addSeparator();
    addAction(mActionExport);
    addSeparator();
    addAction(mActionView);

    retranslateUi();
}

void TPopMenuTrackList::retranslateUi()
{
    mActionPlay->setText(tr("Play"));
    mActionRenameTitle->setText(tr("Rename title"));
    mActionRenameDuration->setText(tr("Rename duration"));
    mActionExport->setText(tr("Export"));
    mActionView->setText(tr("View property"));
}
