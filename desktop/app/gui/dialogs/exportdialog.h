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

    void setMusicFile(QString fileName);
    void setIndexInfo(QString indexName);
    void setIndexInfo(QStringList indexList);
    void setMaxDuration(int microSeconds);
    void setOutputPath(QString fileName);

    QString getMusicFileName();
    QString getOutputDir();
    QStringList getIndexInfo();
    QString getFormat();
    int getMaxDuration();
    bool overWrite();
    bool autoCorrect();

signals:
    void onAddClicked();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_btnChooseDir_clicked();

private:
    Ui::TExportDialog *ui;

    QStringList mIndexList;

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TEXPORTDIALOG_H
