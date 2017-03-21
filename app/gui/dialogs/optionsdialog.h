#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TOptionsDialog;
}

enum PageIndex
{
    PI_GENERAL,
    PI_PLUGIN,
    PI_SKIN,
    PI_ABOUT,

    PI_COUNT
};

class TOptionsDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TOptionsDialog(QWidget *parent = 0);
    ~TOptionsDialog();

    void openPage(PageIndex pageIndex);

private:
    Ui::TOptionsDialog *ui;
    QList<QWidget*> mPages;

    void locatePage(PageIndex pageIndex);

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_listView_clicked(const QModelIndex &index);
};

#endif // OPTIONSDIALOG_H
