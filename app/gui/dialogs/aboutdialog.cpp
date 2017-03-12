#include "aboutdialog.h"
#include "ui_aboutdialog.h"

TAboutDialog::TAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TAboutDialog)
{
    ui->setupUi(this);
}

TAboutDialog::~TAboutDialog()
{
    delete ui;
}
