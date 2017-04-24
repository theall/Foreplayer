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
#ifndef OPTIONPLUGININFO_H
#define OPTIONPLUGININFO_H

#include "optionpagebase.h"

namespace Ui {
class TOptionPluginInfo;
}

struct TPluginInfo
{
    QString name;
    QString manufacture;
    QString contact;
    QString description;
    QString createDate;
    QString fileName;
    QMap<QString, QString> suffixDesc;
};

typedef QList<TPluginInfo*> TPluginInfos;

class TOptionPluginInfo : public TOptionPageBase
{
    Q_OBJECT

public:
    explicit TOptionPluginInfo(QWidget *parent = 0);
    ~TOptionPluginInfo();

signals:
    void requestPluginInformation(TPluginInfos&);

private:
    Ui::TOptionPluginInfo *ui;
    TPluginInfos mPluginInfos;

    // TOptionPageBase interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // OPTIONPLUGININFO_H
