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
#ifndef TABSTRACTTABLEVIEW_H
#define TABSTRACTTABLEVIEW_H

#include "../../../widgets/scrollbar.h"

class TTableViewDelegate : public QStyledItemDelegate
{
    static QPixmap *mSelectedPixmap;

public:
    explicit TTableViewDelegate(QObject *parent = 0);

    static void setSelectedPixmap(QPixmap *pixmap);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void destroyEditor(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

class TAbstractTableView : public QTableView
{
    Q_OBJECT

public:
    TAbstractTableView(QWidget *parent = 0);

    static void setBackgroundColor(QColor color);
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;
    void addFiles(QStringList files, int pos = 0x7fffffff);
    void addFiles(QList<QUrl> urls, int pos = 0x7fffffff);
    QList<int> selectedRows();
    int currentRow();
    void selectRows(QList<int> rows, bool locate = true);

signals:
    void onCurrentRowChanged(int row);
    void onDoubleClickItem(int row);
    void requestMoveItems(QList<int> indexes, int pos, QList<int> &newIndexes);
    void requestAddFiles(QStringList files, int pos, QList<int> &newIndexes);

    void onKeyPressed(int key, bool& processed);
    // This message post to playlist window
    void onMouseMove(QEvent *ev);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    virtual void updateColumnsWidth() = 0;

private slots:

private:
    static QColor mBackground;
    static QColor mHighlightColor;
    QRect mHighlightRect;
    bool mColumnsWidthResized;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
};

#endif // TABSTRACTTABLEVIEW_H
