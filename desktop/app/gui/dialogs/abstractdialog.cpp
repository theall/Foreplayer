#include "abstractdialog.h"

TAbstractDialog::TAbstractDialog(QWidget *parent, bool resizable) :
    QDialog(parent)
{
    Qt::WindowFlags flags = windowFlags();
    flags ^= Qt::WindowContextHelpButtonHint;
    if(!resizable)
        flags |= Qt::MSWindowsFixedSizeDialogHint;
    else
        flags |= Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;

    setWindowFlags(flags);

    setModal(true);
}

TAbstractDialog::~TAbstractDialog()
{

}

bool TAbstractDialog::event(QEvent *event)
{
    if(event->type()==QEvent::LanguageChange)
    {
        retranslateUi();
    }
    return QDialog::event(event);
}

void TAbstractDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}
