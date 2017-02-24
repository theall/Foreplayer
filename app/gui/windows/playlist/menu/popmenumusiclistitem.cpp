#include "popmenumusiclistitem.h"

TPopMenuMusiclistItem::TPopMenuMusiclistItem(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionPlay = new QAction(this);
    mActionCopy = new QAction(this);
    mActionCut = new QAction(this);
    mActionPaste = new QAction(this);
    mActionDelete = new QAction(this);
    mActionRename = new QAction(this);
    mActionExplorer = new QAction(this);
    mActionExport = new QAction(this);
    mActionDetail = new QAction(this);

    connect(mActionPlay, SIGNAL(triggered()), this, SIGNAL(onActionPlayTriggered()));
    connect(mActionCopy, SIGNAL(triggered()), this, SIGNAL(onActionCopyTriggered()));
    connect(mActionCut, SIGNAL(triggered()), this, SIGNAL(onActionCutTriggered()));
    connect(mActionPaste, SIGNAL(triggered()), this, SIGNAL(onActionPasteTriggered()));
    connect(mActionDelete, SIGNAL(triggered()), this, SIGNAL(onActionDeleteTriggered()));
    connect(mActionRename, SIGNAL(triggered()), this, SIGNAL(onActionRenameTriggered()));
    connect(mActionExplorer, SIGNAL(triggered()), this, SIGNAL(onActionExplorerTriggered()));
    connect(mActionExport, SIGNAL(triggered()), this, SIGNAL(onActionExportTriggered()));
    connect(mActionDetail, SIGNAL(triggered()), this, SIGNAL(onActionDetailTriggered()));

    addAction(mActionPlay);
    addSeparator();
    addAction(mActionCopy);
    addAction(mActionCut);
    addAction(mActionPaste);
    addSeparator();
    addAction(mActionDelete);
    addSeparator();
    addAction(mActionRename);
    addSeparator();
    addAction(mActionExplorer);
    addAction(mActionExport);
    addSeparator();
    addAction(mActionDetail);
}

TPopMenuMusiclistItem::~TPopMenuMusiclistItem()
{

}

void TPopMenuMusiclistItem::retranslateUi()
{
    mActionCut->setText(tr("Cut"));
    mActionCopy->setText(tr("Copy"));
    mActionPlay->setText(tr("Play"));
    mActionPaste->setText(tr("Paste"));
    mActionDelete->setText(tr("Delete"));
    mActionDetail->setText(tr("Detail"));
    mActionExport->setText(tr("Export"));
    mActionRemove->setText(tr("Remove"));
    mActionExplorer->setText(tr("Explorer"));
    mActionRename->setText(tr("Rename title"));
    mActionDetail->setText(tr("View property"));
    mActionCopyToUsb->setText(tr("Copy to storage"));
}
