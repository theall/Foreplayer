#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TPropertyDialog;
}

class TPropertyDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TPropertyDialog(QWidget *parent = 0);
    ~TPropertyDialog();

    void setMusicFile(QString fileName);
    void setTitle(QString title);
    void setAuthor(QString author);
    void setYear(QString year);
    void setSystem(QString system);
    void setAddionalInfo(QString other);
    void setIndex(int current, int total);

signals:
    void onPreviousClicked();
    void onNextClicked();

private slots:
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();
    void on_btnClose_clicked();

private:
    Ui::TPropertyDialog *ui;

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // PROPERTYDIALOG_H
