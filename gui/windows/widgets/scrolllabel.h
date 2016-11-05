#ifndef TSCROLLLABEL_H
#define TSCROLLLABEL_H

#include "pch.h"

class TScrollLabel : public QWidget
{
    Q_OBJECT
public:
    explicit TScrollLabel(QWidget *parent = 0);

    void setStrings(QStringList strlist);
    void setFontColor(QFont font, QColor color);

signals:

public slots:

// QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *event);

private:
    enum DrawStatus {
        START,
        IDLE,
        SWITCH,
        SHUTTLE_LEFT,
        IDLE_SHORT,
        SHUTTLE_RIGHT,
        BREAK,
        END
    };

    int mLineWidth;
    bool mSwitchEnd;
    int mCurrentIndex;
    QString mLine1;
    QString mLine2;
    QString mLine;
    QStringList mStrings;
    int mOffsetX, mOffsetY;
    int mIndex;
    DrawStatus mStatus;
    int mTimerID;
    int mIdleTime;
    void prepareNext();
};

#endif // TSCROLLLABEL_H
