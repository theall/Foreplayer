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
    void onActionPlayTriggered();
    void onActionCopyTriggered();
    void onActionCutTriggered();
    void onActionPasteTriggered();
    void onActionDeleteTriggered();
    void onActionRenameTriggered();
    void onActionExplorerTriggered();
    void onActionExportTriggered();
    void onActionDetailTriggered();

private:
    QAction *mActionCopy;
    QAction *mActionCopyToUsb;
    QAction *mActionCut;
    QAction *mActionDelete;
    QAction *mActionDetail;
    QAction *mActionExplorer;
    QAction *mActionExport;
    QAction *mActionPaste;
    QAction *mActionPlay;
    QAction *mActionRemove;
    QAction *mActionRename;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUMUSICLISTITEM_H
