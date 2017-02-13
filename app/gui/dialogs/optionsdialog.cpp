#include "optionsdialog.h"
#include "ui_optionsdialog.h"

TOptionsDialog::TOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TOptionsDialog)
{
    ui->setupUi(this);
}

TOptionsDialog::~TOptionsDialog()
{
    delete ui;
}
