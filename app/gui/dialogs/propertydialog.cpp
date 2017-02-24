#include "propertydialog.h"
#include "ui_propertydialog.h"

TPropertyDialog::TPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TPropertyDialog)
{
    ui->setupUi(this);
}

TPropertyDialog::~TPropertyDialog()
{
    delete ui;
}
