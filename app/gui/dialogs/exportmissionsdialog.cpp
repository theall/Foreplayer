#include "exportmissionsdialog.h"
#include "ui_exportmissionsdialog.h"

TExportMissionsDialog::TExportMissionsDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TExportMissionsDialog)
{
    ui->setupUi(this);
}

TExportMissionsDialog::~TExportMissionsDialog()
{
    delete ui;
}
