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
