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
#include "abstracttableview.h"

#include "utils.h"
#include <QMimeData>

#define DEFAULT_ROW_HEIGHT      20

QColor TAbstractTableView::mBackground;
QColor TAbstractTableView::mHighlightColor;
QPixmap *TTableViewDelegate::mSelectedPixmap = NULL;

QModelIndex g_editingIndex;

TTableViewDelegate::TTableViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void TTableViewDelegate::setSelectedPixmap(QPixmap *pixmap)
{
    TTableViewDelegate::mSelectedPixmap = pixmap;
}

QWidget *TTableViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
    QPalette pal = editor->palette();
    pal.setBrush(QPalette::Text, QBrush(index.data(Utils::TextHighlight).value<QColor>()));
    editor->setPalette(pal);
    g_editingIndex = index;
    return editor;
}

void TTableViewDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::destroyEditor(editor, index);

    g_editingIndex = QModelIndex();
}

void TTableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    int rowCount = model->rowCount()-1;
    int columnCount = model->columnCount();
    int columnIndex = index.column();
    if(index.row() >= rowCount)
        return;

    painter->save();
    QRect rect = option.rect;
    bool isPlayingItem = index.data(Utils::IsPlayingItem).toBool();
    //bool isCurrentRow = index.data(Utils::IsCurrentRow).toBool();

    if(option.state & QStyle::State_Selected)
    {
        // Draw background
        rect.adjust(0, 0, -1, -1);
        painter->drawPixmap(option.rect, *TTableViewDelegate::mSelectedPixmap);

        // Draw border
        if(true)
        {
            painter->setPen(Qt::white);
            if(columnCount>1)
            {

                rect = option.rect;
                painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
                painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());

                if(columnIndex==0)
                {
                    painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
                } else if(columnIndex==columnCount-1) {
                    painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
                }
            } else {
                painter->drawRect(rect);
            }
        }
    } else {
        painter->fillRect(rect, index.data(Qt::BackgroundColorRole).value<QBrush>());
    }

    // Set pen color
    QColor textColor;
    if(option.state&QStyle::State_Selected)
        textColor = index.data(Utils::TextHighlight).value<QColor>();
    else
    {
        if(/*isCurrentRow || */isPlayingItem)
            textColor = index.data(Utils::TextHighlight).value<QColor>();
        else
            textColor = index.data(Qt::TextColorRole).value<QColor>();
    }

    painter->setPen(textColor);

    // Draw triangle icon
    if(isPlayingItem && columnCount>1 && columnIndex==0)
    {
        QRect tRect = option.rect;
        tRect.adjust(2, 1, -1, -1);
        tRect.setWidth(4);

        QPolygonF triangle;
        QPainterPath trianglePath;

        int left = tRect.left();
        int right = tRect.right();
        int middle = tRect.top() + tRect.height()/2;

        // Draw triangle
        triangle.append(QPoint(left, middle-3));
        triangle.append(QPoint(right, middle));
        triangle.append(QPoint(left, middle+3));
        trianglePath.addPolygon(triangle);

        painter->fillPath(trianglePath, QBrush(textColor));
    }

    if(g_editingIndex != index) // Disable draw text if current item is in editing
    {
        // Draw text
        QString text = index.data().toString();
        Qt::Alignment alignment = (Qt::Alignment)model->data(index, Qt::TextAlignmentRole).toInt();
        QTextOption textOption;
        textOption.setWrapMode(QTextOption::NoWrap);
        textOption.setAlignment(alignment);
        rect.adjust(2, 0, -4, 0);
        painter->setFont(model->data(index, Qt::FontRole).value<QFont>());
        painter->drawText(rect, text, textOption);
    }

    painter->restore();
}

TAbstractTableView::TAbstractTableView(QWidget *parent) :
    QTableView(parent),
    mColumnsWidthResized(false)
{
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);

    setShowGrid(false);
    setAutoFillBackground(false);

    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(DEFAULT_ROW_HEIGHT);

    setTextElideMode(Qt::ElideRight);

    QPalette pll = palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    setPalette(pll);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setItemDelegate(new TTableViewDelegate(this));

    TScrollBar *bar = new TScrollBar(Qt::Vertical, this);
    connect(bar, &TScrollBar::onVisibleToggle, this, &TAbstractTableView::updateColumnsWidth);
    setVerticalScrollBar(bar);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    viewport()->installEventFilter(this);

    setMouseTracking(true);
}

void TAbstractTableView::setBackgroundColor(QColor color)
{
    mBackground = color;
    QRgb rgb = color.rgba();
    rgb = ~rgb;
    mHighlightColor.setRgba(rgb);
    mHighlightColor.setAlpha(255);
}

void TAbstractTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    resizeColumnsToContents();
}

void TAbstractTableView::addFiles(QStringList files, int pos)
{
    if(files.count() > 0)
    {
        QList<int> newIndexes;
        emit requestAddFiles(files, pos, newIndexes);
        selectRows(newIndexes);
    }
}

void TAbstractTableView::addFiles(QList<QUrl> urls, int pos)
{
    QStringList files;
    for(QUrl url : urls) {
        files.append(url.toLocalFile());
    }
    addFiles(files, pos);
}

void TAbstractTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    updateColumnsWidth();
}

void TAbstractTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);

    QPainter painter(viewport());
    if (mHighlightRect.isValid()) {
        painter.setBrush(TAbstractTableView::mHighlightColor);
        painter.drawRect(mHighlightRect);
    }

    painter.end();
}

QList<int> TAbstractTableView::selectedRows()
{
    QSet<int> selected;
    for(QModelIndex i : selectedIndexes()) {
        selected.insert(i.row());
    }
    return selected.toList();
}

int TAbstractTableView::currentRow()
{
    QModelIndex i = currentIndex();
    if(i.isValid())
        return i.row();
    return -1;
}

void TAbstractTableView::selectRows(QList<int> rows, bool locate)
{
    QItemSelectionModel *selModel = selectionModel();
    selModel->clearSelection();
    QAbstractItemModel *m = model();
    int indexSize = rows.size();
    if(!m || !selModel || indexSize<=0)
        return;

    int columns = m->columnCount();
    for(int i : rows)
        for(int j=0;j<columns;j++)
            selModel->select(m->index(i, j), QItemSelectionModel::Select);

    if(locate)
    {
        scrollTo(m->index(rows.takeLast(), 0), QAbstractItemView::PositionAtCenter);
    }
}

void TAbstractTableView::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->source()==this || event->mimeData()->hasUrls()) {
        QModelIndex i = indexAt(event->pos());
        QRect rt;
        if(i.isValid()) {
            rt = visualRect(i);
            rt.setTop(rt.top()-1);
        } else {
            QAbstractItemModel *m = model();
            int rows = m->rowCount()-1;
            i = m->index(rows-1, 0);
            rt = visualRect(i);
            rt.setTop(rt.bottom()+1);
        }
        rt.setLeft(0);
        rt.setRight(width()-1);
        rt.setHeight(2);
        if(rt != mHighlightRect)
        {
            mHighlightRect = rt;
            viewport()->update();
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void TAbstractTableView::dragLeaveEvent(QDragLeaveEvent *event)
{
    QRect updateRect = mHighlightRect;
    mHighlightRect = QRect();
    viewport()->update(updateRect);
    event->accept();
}

void TAbstractTableView::dropEvent(QDropEvent *event)
{
    int insertRow = indexAt(event->pos()).row();
    QAbstractItemModel *m = model();
    if(m)
    {
        if(insertRow < 0)
            insertRow = m->rowCount()-1;

        if(event->source()==this) {
            QList<int> selected = selectedRows();
            if(selected.size() > 0)
            {
                QList<int> newIndexes;
                emit requestMoveItems(selected, insertRow, newIndexes);
                selectRows(newIndexes);
            }
            event->accept();
        } else {
            const QMimeData *mimeData = event->mimeData();
            if (mimeData->hasUrls())
            {
                addFiles(mimeData->urls(), insertRow);
                event->accept();
            } else {
                event->ignore();
            }
        }
    }
    mHighlightRect = QRect();
    viewport()->update();
}

void TAbstractTableView::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        event->accept();
    } else if (event->source()==this) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool TAbstractTableView::eventFilter(QObject *o, QEvent *e)
{
    if(e->type()==QEvent::MouseMove && o==viewport())
    {
        emit onMouseMove(e);
    }

    return QTableView::eventFilter(o, e);
}

void TAbstractTableView::showEvent(QShowEvent *)
{
    if(!mColumnsWidthResized)
    {
        QTimer::singleShot(0, this, &TAbstractTableView::updateColumnsWidth);
        mColumnsWidthResized = true;
    }
}

void TAbstractTableView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    int row = index.row();
    // Last row is stub
    if(row>=0 && row<model()->rowCount()-1)
        emit onCurrentRowChanged(row);
    else {
        clearSelection();
        setCurrentIndex(QModelIndex());
    }

    QTableView::mousePressEvent(event);
}

void TAbstractTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    int row = index.row();
    // Last row is stub
    if(row>=0 && row<model()->rowCount()-1)
        emit onDoubleClickItem(row);

    QTableView::mouseDoubleClickEvent(event);
}

void TAbstractTableView::keyPressEvent(QKeyEvent *event)
{
    if(!g_editingIndex.isValid())
    {
        int key = event->key();
        if(key==Qt::Key_Return || key==Qt::Key_Enter)
        {
            QModelIndex index = selectionModel()->currentIndex();
            int row = index.row();
            // Last row is stub
            if(row>=0 && row<model()->rowCount()-1)
            {
                emit onCurrentRowChanged(row);
                emit onDoubleClickItem(row);
            }
        } else {
            bool processed = false;
            emit onKeyPressed(key|event->modifiers(), processed);
            if(processed)
                return;
        }
    }

    QTableView::keyPressEvent(event);
}
