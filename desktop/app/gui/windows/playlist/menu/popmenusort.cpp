#include "popmenusort.h"

TPopMenuSort::TPopMenuSort(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    mActionSortAsName = addAction(QString(), this, SIGNAL(onActionSortAsNameTriggered()));
    mActionSortAsArtist = addAction(QString(), this, SIGNAL(onActionSortAsArtistTriggered()));
    mActionSortAsAlbum = addAction(QString(), this, SIGNAL(onActionSortAsAlbumTriggered()));
    mActionSortAsDuration = addAction(QString(), this, SIGNAL(onActionSortAsDurationTriggered()));
    addSeparator();
    mActionSortRandom = addAction(QString(), this, SIGNAL(onActionSortRandomTriggered()));
    mActionSortReverse = addAction(QString(), this, SIGNAL(onActionSortReverseTriggered()));
    retranslateUi();
}

TPopMenuSort::~TPopMenuSort()
{

}

void TPopMenuSort::retranslateUi()
{
    setTitle(tr("Sort"));

    mActionSortAsName->setText(tr("As title"));
    mActionSortAsArtist->setText(tr("As artist"));
    mActionSortAsAlbum->setText(tr("As album"));
    mActionSortAsDuration->setText(tr("As length"));
    mActionSortRandom->setText(tr("Random"));
    mActionSortReverse->setText(tr("Reverse"));
}
