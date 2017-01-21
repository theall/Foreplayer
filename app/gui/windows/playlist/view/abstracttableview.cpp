#include "abstracttableview.h"

#include "utils.h"
#include <QMimeData>

#define DEFAULT_ROW_HEIGHT      20

QColor TAbstractTableView::mBackground;
QColor TAbstractTableView::mHighlightColor;
QPixmap *TTableViewDelegate::mSelectedPixmap = NULL;

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
    return editor;
}

void TTableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    int columnCount = model->columnCount();
    int columnIndex = index.column();

    painter->save();
    QRect rect = option.rect;
    bool isPlayingItem = index.data(Utils::IsPlayingItem).toBool();
    bool isCurrentRow = index.data(Utils::IsCurrentRow).toBool();

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
                } else if(columnIndex==2) {
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
        if(isCurrentRow || isPlayingItem)
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

    // Draw text
    QString text = index.data().toString();
    Qt::Alignment alignment = (Qt::Alignment)model->data(index, Qt::TextAlignmentRole).toInt();
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::NoWrap);
    textOption.setAlignment(alignment);
    rect.adjust(2, 0, -4, 0);
    painter->setFont(model->data(index, Qt::FontRole).value<QFont>());
    painter->drawText(rect, text, textOption);

    painter->restore();
}

TAbstractTableView::TAbstractTableView(QWidget *parent) :
    QTableView(parent),
    mColumnsWidthResized(false)
{
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);

    setShowGrid(false);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(DEFAULT_ROW_HEIGHT);

    setTextElideMode(Qt::ElideRight);

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, QBrush(TAbstractTableView::mBackground));
    setPalette(pal);

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
        selectIndexes(newIndexes);
    }
}

void TAbstractTableView::addFiles(QList<QUrl> urls, int pos)
{
    QStringList files;
    foreach (QUrl url, urls) {
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

QSet<int> TAbstractTableView::selectedRows()
{
    QSet<int> selected;
    foreach (QModelIndex i, selectedIndexes()) {
        selected.insert(i.row());
    }
    return selected;
}

void TAbstractTableView::selectIndexes(QList<int> indexes, bool locate)
{
    QItemSelectionModel *selModel = selectionModel();
    QAbstractItemModel *m = model();
    int indexSize = indexes.size();
    if(!m || !selModel || indexSize<=0)
        return;

    int columns = m->columnCount();
    foreach (int i, indexes)
        for(int j=0;j<columns;j++)
            selModel->select(m->index(i, j), QItemSelectionModel::Select);

    if(locate)
    {
        scrollTo(m->index(indexes.takeLast(), 0), QAbstractItemView::PositionAtCenter);
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
            int rows = m->rowCount();
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
    if(!m)
        return;

    if(insertRow < 0)
        insertRow = m->rowCount();

    if(event->source()==this) {
        QList<int> selected = selectedRows().toList();
        if(selected.size() > 0)
        {
            QList<int> newIndexes;
            emit requestMoveItems(selected, insertRow, newIndexes);
            selectIndexes(newIndexes);
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
    if(row >= 0)
        emit onCurrentRowChanged(row);

    QTableView::mousePressEvent(event);
}

void TAbstractTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    int row = index.row();
    if(row >= 0)
        emit onDoubleClickItem(row);

    QTableView::mouseDoubleClickEvent(event);
}
