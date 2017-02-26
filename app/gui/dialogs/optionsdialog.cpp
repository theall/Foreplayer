#include "optionsdialog.h"
#include "ui_optionsdialog.h"

TOptionsDialog::TOptionsDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TOptionsDialog)
{
    ui->setupUi(this);
}

TOptionsDialog::~TOptionsDialog()
{
    delete ui;
}
