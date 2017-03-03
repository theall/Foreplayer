#include "optionsdialog.h"
#include "ui_optionsdialog.h"

TOptionsDialog::TOptionsDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TOptionsDialog)
{
    ui->setupUi(this);

    retranslateUi();
}

TOptionsDialog::~TOptionsDialog()
{
    delete ui;
}

void TOptionsDialog::retranslateUi()
{
    ui->retranslateUi(this);

}
