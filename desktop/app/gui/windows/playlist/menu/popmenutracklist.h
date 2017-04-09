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
    void onActionChangeTriggered();
    void onActionExportTriggered();
    void onActionViewTriggered();

private:
    QAction *mActionPlay;
    QAction *mActionCopy;
    QAction *mActionChange;
    QAction *mActionExport;
    QAction *mActionView;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};
#endif // TPOPMENUTRACKLIST_H
