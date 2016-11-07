#include "abstracttableview.h"
#include "utils.h"

QColor TAbstractTableView::mBackground;

TTableViewDelegate::TTableViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    mSelectedPixmap.load("z:/skins/fulkfour/playlist_selected.bmp");
}

void TTableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    int columnCount = model->columnCount();
    int columnIndex = index.column();

    painter->save();
    QRect rect = option.rect;

    if(option.state & QStyle::State_Selected)
    {
        // Draw background
        rect.adjust(0, 0, -1, -1);
        painter->drawPixmap(option.rect, mSelectedPixmap);

        // Draw border
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

            if(columnIndex==0)
            {

            }
        } else {
            painter->drawRect(rect);
        }
    } else {
        painter->fillRect(rect, index.data(Qt::BackgroundColorRole).value<QBrush>());
    }

    // Set pen color
    bool isCurrentRow = index.data(Utils::IsCurrentRow).toBool();

    QColor textColor;
    if(option.state&QStyle::State_Selected)
        textColor = Qt::white;
    else
        textColor = index.data(isCurrentRow?Utils::TextHighlight:Qt::TextColorRole).value<QColor>();

    painter->setPen(textColor);

    // Draw triangle icon
    if(columnCount>1 && columnIndex==0 && isCurrentRow)
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
    rect.adjust(3, 0, -4, 0);
    painter->setFont(model->data(index, Qt::FontRole).value<QFont>());
    painter->drawText(rect, text, textOption);

    painter->restore();
}

TAbstractTableView::TAbstractTableView(QWidget *parent) :
    QTableView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);

    setShowGrid(false);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    verticalHeader()->setDefaultSectionSize(20);

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
}

void TAbstractTableView::setBackgroundColor(QColor color)
{
    mBackground = color;
}

void TAbstractTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    resizeColumnsToContents();
}

void TAbstractTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    updateColumnsWidth();
}

void TAbstractTableView::focusOutEvent(QFocusEvent *event)
{
    QTableView::focusOutEvent(event);

    clearSelection();
}
