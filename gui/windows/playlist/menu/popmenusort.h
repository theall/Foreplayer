#ifndef TPOPMENUSORT_H
#define TPOPMENUSORT_H

#include "abstractpopmenu.h"

class TPopMenuSort : public TAbstractPopMenu
{
    Q_OBJECT

public:
    TPopMenuSort(QWidget *parent = 0);
    ~TPopMenuSort();

signals:
    void onActionSortAsNameTriggered();
    void onActionSortAsArtistTriggered();
    void onActionSortAsAlbumTriggered();
    void onActionSortAsDurationTriggered();
    void onActionSortRandomTriggered();
    void onActionSortReverseTriggered();

private:
    QAction *mActionSortAsName;
    QAction *mActionSortAsArtist;
    QAction *mActionSortAsAlbum;
    QAction *mActionSortAsDuration;
    QAction *mActionSortRandom;
    QAction *mActionSortReverse;

    // TAbstractPopMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TPOPMENUSORT_H
