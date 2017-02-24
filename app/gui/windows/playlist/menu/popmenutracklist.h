#ifndef TPOPMENUTRACKLIST_H
#define TPOPMENUTRACKLIST_H

#include "abstractpopmenu.h"

class TPopMenuTrackList : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuTrackList(QWidget *parent = 0);

signals:
    void onActionPlayTriggered();
    void onActionCopyTriggered();
    void onActionCutTriggered();
    void onActionPasteTriggered();
    void onActionDeleteTriggered();
    void onActionRenameTitleTriggered();
    void onActionRenameDurationTriggered();
    void onActionExplorerTriggered();
    void onActionExportTriggered();
    void onActionViewTriggered();

private:
    QAction *mActionPlay;
    QAction *mActionCopy;
    QAction *mActionCut;
    QAction *mActionPaste;
    QAction *mActionDelete;
    QAction *mActionRenameTitle;
    QAction *mActionRenameDuration;
    QAction *mActionExplorer;
    QAction *mActionExport;
    QAction *mActionView;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};
#endif // TPOPMENUTRACKLIST_H
