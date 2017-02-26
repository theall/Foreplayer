#ifndef TEXPORTDIALOG_H
#define TEXPORTDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TExportDialog;
}

class TExportDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TExportDialog(QWidget *parent = 0);
    ~TExportDialog();

private:
    Ui::TExportDialog *ui;
};

#endif // TEXPORTDIALOG_H
