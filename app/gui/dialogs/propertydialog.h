#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
class TPropertyDialog;
}

class TPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TPropertyDialog(QWidget *parent = 0);
    ~TPropertyDialog();

private:
    Ui::TPropertyDialog *ui;
};

#endif // PROPERTYDIALOG_H
