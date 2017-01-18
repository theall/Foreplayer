#ifndef TPOPMENUMUSICLISTEDIT_H
#define TPOPMENUMUSICLISTEDIT_H

#include "abstractpopmenu.h"

class TPopMenuMusiclistEdit : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuMusiclistEdit(QWidget *parent = 0);
    ~TPopMenuMusiclistEdit();

signals:
    void onActionCopyTriggered();
    void onActionPasteTriggered();
    void onActionRemoveTriggered();
    void onActionSelectAllTriggered();
    void onActionUnSelectAllTriggered();
    void onActionInverseTriggered();

private:
    QAction *mActionCopy;
    QAction *mActionPaste;
    QAction *mActionRemove;
    QAction *mActionSelectAll;
    QAction *mActionUnSelectAll;
    QAction *mActionInverse;
    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUMUSICLISTEDIT_H
