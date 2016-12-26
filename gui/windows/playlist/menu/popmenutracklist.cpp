#include "popmenutracklist.h"

TPopMenuTrackList::TPopMenuTrackList(QWidget *parent) :
    TAbstractPopMenu(parent)
{
    retranslateUi();
}

TPopMenuTrackList::~TPopMenuTrackList()
{

}

void TPopMenuTrackList::retranslateUi()
{
    setTitle(tr("Tracklist"));
}
