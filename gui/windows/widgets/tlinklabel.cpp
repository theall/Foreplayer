#include "tlinklabel.h"

TLinkLabel::TLinkLabel(QWidget *parent):
    TLabel(parent)
{

}


void TLinkLabel::setFontColor(QFont font, QColor color)
{
    font.setUnderline(true);
    TLabel::setFontColor(font, color);
}
