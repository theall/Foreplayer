/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef TEXPORTMISSIONSDIALOG_H
#define TEXPORTMISSIONSDIALOG_H

#include "abstractdialog.h"

#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QMenu>
#include <QItemSelectionModel>

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
    void requestPauseMissions(QList<int>);
    void queryMissionsStatus(QList<int>, QList<bool>&);
    void requestStartMissions(QList<int>);
    void requestRemoveMissions(QList<int>);
    void requestExploreFiles(QList<int>);
    void requestOpenSettingDialog();

private slots:
    void slotOnCustomizedContextMenu(QPoint);
    void slotOnActionStartTriggered();
    void slotOnActionPauseTriggered();
    void slotOnActionDeleteTriggered();
    void slotOnActionExploreTriggered();
    void slotOnSelectionChanged(QItemSelection,QItemSelection);

    void on_btnSetting_clicked();
    void on_btnClose_clicked();
    void on_btnStart_clicked();
    void on_btnDelete_clicked();
    void on_btnPause_clicked();

private:
    Ui::TExportMissionsDialog *ui;
    QList<int>  getSelectedRows();
    QMenu *mContextMenu;
    QAction *mActionStart;
    QAction *mActionPause;
    QAction *mActionDelete;
    QAction *mActionExplore;

    void createContextMenu();
    void updateUI();

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TEXPORTMISSIONSDIALOG_H
