#ifndef TPOPMENUTRACKLIST_H
#define TPOPMENUTRACKLIST_H

#include "abstractpopmenu.h"

class TPopMenuTrackList : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuTrackList(QWidget *parent = 0);
    ~TPopMenuTrackList();

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUTRACKLIST_H
