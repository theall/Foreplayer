#include "abstractpopmenu.h"

TAbstractPopMenu::TAbstractPopMenu(QWidget *parent) :
    QMenu(parent)
{

}

TAbstractPopMenu::~TAbstractPopMenu()
{

}

bool TAbstractPopMenu::event(QEvent *event)
{
    if(event->type()==QEvent::LanguageChange)
    {
        retranslateUi();
    }
    return QMenu::event(event);
}
