#ifndef TPOPMENUMUSICLISTITEM_H
#define TPOPMENUMUSICLISTITEM_H

#include "abstractpopmenu.h"

class TPopMenuMusiclistItem : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuMusiclistItem(QWidget *parent = 0);
    ~TPopMenuMusiclistItem();

signals:
    void onActionReparseTriggered();
    void onActionPlayTriggered();
    void onActionCopyTriggered();
    void onActionCutTriggered();
    void onActionPasteTriggered();
    void onActionDeleteTriggered();
    void onActionRenameTriggered();
    void onActionExplorerTriggered();
    void onActionExportTriggered();
    void onActionDetailTriggered();
    void onActionCopyToUsbTriggered();

private:
    QAction *mActionCopy;
    QAction *mActionCopyToUsb;
    QAction *mActionCut;
    QAction *mActionRemove;
    QAction *mActionDetail;
    QAction *mActionExplorer;
    QAction *mActionExport;
    QAction *mActionPaste;
    QAction *mActionPlay;
    QAction *mActionRename;
    QAction *mActionReparse;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUMUSICLISTITEM_H
