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
#ifndef OPTIONSKIN_H
#define OPTIONSKIN_H

#include "optionpagebase.h"

#include <QShowEvent>

namespace Ui {
class TOptionSkin;
}

class TOptionSkin : public TOptionPageBase
{
    Q_OBJECT

public:
    explicit TOptionSkin(QWidget *parent = 0);
    ~TOptionSkin();

signals:
    void requestLoadSkin(int index);
    void requestSkinNames(QStringList &skinNames, int &currentIndex);
    void requestSkinInfo(int index, QPixmap &preview, QString &author, QString &website, QString &contact);

private slots:
    void slotCurrentSkinIndexChanged(QModelIndex,QModelIndex);

private:
    Ui::TOptionSkin *ui;

    // QWidget interface
protected:
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
private slots:
    void on_btnApply_clicked();

    // TOptionPageBase interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // OPTIONSKIN_H
