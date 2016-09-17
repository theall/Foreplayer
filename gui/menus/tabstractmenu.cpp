#include "tabstractmenu.h"

TAbstractMenu::TAbstractMenu(QWidget *parent) :
    QMenu(parent)
{

}


bool TAbstractMenu::event(QEvent *event)
{
    if(event->type()==QEvent::LanguageChange)
    {
        retranslateUi();
    }
    return QMenu::event(event);
}
