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
    void onActionDetailTriggered();
    void onActionRemoveTriggered();
    void onActionCopyTriggered();
    void onActionPasteTriggered();
    void onActionCopyToUsbTriggered();

private:
    QAction *mActionPlay;
    QAction *mActionDetail;
    QAction *mActionRemove;
    QAction *mActionCopy;
    QAction *mActionPaste;
    QAction *mActionCopyToUsb;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUMUSICLISTITEM_H
