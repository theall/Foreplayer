#ifndef TABSTRACTDIALOG_H
#define TABSTRACTDIALOG_H

#include <QDialog>

class TAbstractDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TAbstractDialog(QWidget *parent = 0);
    ~TAbstractDialog();

private:
};

#endif // TABSTRACTDIALOG_H
