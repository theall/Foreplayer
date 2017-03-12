#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class TAboutDialog;
}

class TAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TAboutDialog(QWidget *parent = 0);
    ~TAboutDialog();

private:
    Ui::TAboutDialog *ui;
};

#endif // ABOUTDIALOG_H
