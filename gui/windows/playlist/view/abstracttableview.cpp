#include "abstracttableview.h"

QColor TAbstractTableView::mBackground;

TTableViewDelegate::TTableViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    mSelectedPixmap.load("z:/skins/fulkfour/playlist_selected.bmp");
}

void TTableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(option.state & QStyle::State_Selected)
    {
        painter->save();

        QRect rect = option.rect;
        const QAbstractItemModel *model = index.model();
        QString text = model->data(index).toString();
        Qt::Alignment alignment = (Qt::Alignment)model->data(index, Qt::TextAlignmentRole).toInt();
        QTextOption textOption;
        textOption.setAlignment(alignment);

        int columnCount = model->columnCount();
        rect.adjust(0, 0, -1, -1);
        painter->drawPixmap(option.rect, mSelectedPixmap);

        painter->setPen(Qt::white);

        if(columnCount>1)
        {
            rect = option.rect;
            painter->drawLine(rect.left(), rect.top(), rect.right(), rect.top());
            painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());

            int columnIndex = index.column();
            if(columnIndex==0)
            {
                painter->drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());
            } else if(columnIndex==2) {
                painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
            }
        } else {
            painter->drawRect(rect);
        }

        rect.adjust(3, 0, -4, 0);
        painter->drawText(rect, text, textOption);

        painter->restore();
        return;
    }
    QStyledItemDelegate::paint(painter, option, index);
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
