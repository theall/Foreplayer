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
#ifndef TBROWSERWINDOW_H
#define TBROWSERWINDOW_H

#include <QDomElement>

#include "../widgets/linklabel.h"
#include "../widgets/checkbox.h"
#include "../widgets/imagebutton.h"
#include "abstractwindow.h"

class TBrowserWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TBrowserWindow(QWidget *parent = 0);
    ~TBrowserWindow();

signals:
    void browserWindowToggled(bool);

public slots:

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_btnClose_clicked();

private:
    TImageButton *mBtnClose;
    TImageButton *mBtnBackward;
    TImageButton *mBtnForward;
    TImageButton *mBtnRefresh;
    TImageButton *mBtnBrowser;
    TImageButton *mBtnMini;
    TImageButton *mBtnDownload;
    TCheckBox *mCkbStartup;
    TLinkLabel *mLabelLink;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TBROWSERWINDOW_H
