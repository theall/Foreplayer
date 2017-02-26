#ifndef TEXPORTMISSIONSDIALOG_H
#define TEXPORTMISSIONSDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TExportMissionsDialog;
}

class TExportMissionsDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TExportMissionsDialog(QWidget *parent = 0);
    ~TExportMissionsDialog();

private:
    Ui::TExportMissionsDialog *ui;
};

#endif // TEXPORTMISSIONSDIALOG_H
