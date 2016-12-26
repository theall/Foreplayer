#ifndef TPOPMENUADDMUSICS_H
#define TPOPMENUADDMUSICS_H

#include "abstractpopmenu.h"

class TPopMenuAddMusics : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuAddMusics(QWidget *parent = 0);
    ~TPopMenuAddMusics();

signals:
    void onActionAddMusicsTriggered();
    void onActionAddDirectoryTriggered();

private:
    QAction *mActionAddMusics;
    QAction *mActionAddDirectory;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUADDMUSICS_H
