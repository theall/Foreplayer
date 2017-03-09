#ifndef TEXPORTMISSIONSDIALOG_H
#define TEXPORTMISSIONSDIALOG_H

#include "abstractdialog.h"

#include <QAbstractTableModel>
#include <QItemDelegate>

class TExportItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TExportItemDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

namespace Ui {
class TExportMissionsDialog;
}

class TExportMissionsDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TExportMissionsDialog(QWidget *parent = 0);
    ~TExportMissionsDialog();

    void setModel(QAbstractTableModel *model);

signals:
    void requestCloseWindow();
    void requestPauseMissions(QList<int> rows);
    void requestStartMissions(QList<int> rows);
    void requestRemoveMissions(QList<int> rows);

private slots:
    void on_btnSetting_clicked();

private:
    Ui::TExportMissionsDialog *ui;

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TEXPORTMISSIONSDIALOG_H
