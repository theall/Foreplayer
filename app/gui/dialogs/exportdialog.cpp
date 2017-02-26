#include "exportdialog.h"
#include "ui_exportdialog.h"

TExportDialog::TExportDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TExportDialog)
{
    ui->setupUi(this);
}

TExportDialog::~TExportDialog()
{
    delete ui;
}

