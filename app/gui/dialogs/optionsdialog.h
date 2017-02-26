#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TOptionsDialog;
}

class TOptionsDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TOptionsDialog(QWidget *parent = 0);
    ~TOptionsDialog();

private:
    Ui::TOptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
