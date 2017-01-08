#ifndef TABSTRACTTABLEVIEW_H
#define TABSTRACTTABLEVIEW_H

#include "../../widgets/scrollbar.h"

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
};

class TAbstractTableView : public QTableView
{
    Q_OBJECT

public:
    TAbstractTableView(QWidget *parent = 0);

    static void setBackgroundColor(QColor color);
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:
    void onCurrentRowSelected(int index);
    void requestMoveItems(QList<int> indexes, int insertPos, QList<int> &newIndexes);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void updateColumnsWidth() = 0;

private slots:

private:
    static QColor mBackground;
    static QColor mHighlightColor;
    QRect mHighlightRect;
};

#endif // TABSTRACTTABLEVIEW_H
