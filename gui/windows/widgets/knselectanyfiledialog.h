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

#ifndef KNSELECTANYFILEDIALOG_H
#define KNSELECTANYFILEDIALOG_H

#include "knfiledialog.h"

class QDialogButtonBox;
class Q_DECL_EXPORT KNSelectAnyFileDialog : public KNFileDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNSelectAnyFileDialog widget.
     * \param parent The parent widget.
     * \param caption The caption of the dialog.
     * \param directory
     * \param filter
     */
    explicit KNSelectAnyFileDialog(QWidget *parent = 0,
                                   const QString &caption = QString(),
                                   const QString &directory = QString(),
                                   const QString &filter = QString());

signals:

public slots:

private slots:
    void treeViewSelectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);

private:
    QDialogButtonBox *m_buttonBox;
};

#endif // KNSELECTANYFILEDIALOG_H
