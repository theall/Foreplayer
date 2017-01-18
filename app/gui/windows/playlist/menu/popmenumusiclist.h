#ifndef TPOPMENUMUSICLIST_H
#define TPOPMENUMUSICLIST_H

#include "abstractpopmenu.h"

class TPopMenuMusicList : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuMusicList(QWidget *parent = 0);
    ~TPopMenuMusicList();

signals:

private:

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUMUSICLIST_H
