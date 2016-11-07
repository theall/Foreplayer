#ifndef TSPLITTER_H
#define TSPLITTER_H

#include "imagebutton.h"

class TSplitterHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    TSplitterHandle(int index, QSplitter *parent = 0);

    /*
     * When splitter move from collapsed, set the handle to expand.
     */
    void setHandleStatus(bool collapsed);

signals:
    void requestCollapse(int index, bool collapse);

private:
    int mIndex;
    bool mCollapsed;
    bool mouseInButton(int pos);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void hideEvent(QHideEvent *) Q_DECL_OVERRIDE;
};

class TSplitter : public QSplitter
{
    Q_OBJECT

public:
    TSplitter(QWidget *parent = 0);

private slots:
    void slotCollapseWidget(int mIndex, bool collapse);
    void slotSplitterMoved(int pos, int index);

private:
    int mIndex;
    QMap<QWidget*, int> mWidgetWidth;

protected:
    QSplitterHandle *createHandle() Q_DECL_OVERRIDE;
};

#endif // TSPLITTER_H
