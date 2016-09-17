/*
 * Copyright (C) Kreogist Dev Team
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNFILEDIALOG_H
#define KNFILEDIALOG_H

#include <QFileDialog>

class Q_DECL_EXPORT KNFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit KNFileDialog(QWidget *parent = 0,
                          const QString &caption = QString(),
                          const QString &directory = QString(),
                          const QString &filter = QString());

signals:

public slots:
};

#endif // KNFILEDIALOG_H
