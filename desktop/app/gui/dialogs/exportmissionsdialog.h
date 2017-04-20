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
