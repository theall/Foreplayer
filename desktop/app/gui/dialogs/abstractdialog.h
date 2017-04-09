#ifndef TABSTRACTDIALOG_H
#define TABSTRACTDIALOG_H

#include <QDialog>
#include <QCloseEvent>

class TAbstractDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TAbstractDialog(QWidget *parent = 0, bool resizable = false);
    ~TAbstractDialog();

    virtual void retranslateUi() = 0;

    // QObject interface
public:
    bool event(QEvent *) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;
};

#endif // TABSTRACTDIALOG_H
