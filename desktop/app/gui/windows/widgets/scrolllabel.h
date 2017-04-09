#ifndef TSCROLLLABEL_H
#define TSCROLLLABEL_H

#include "../../share/skin.h"

#include <QMutex>

class TScrollLabel : public QWidget, TSkinReader
{
    Q_OBJECT
public:
    explicit TScrollLabel(QWidget *parent = 0);

    void setStrings(QStringList strlist);
    void setFontColor(QFont font, QColor color);
    void setSwitchOnClick(bool enabled);
    void setSpeed(int factor);

signals:

public slots:

// QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    enum State {
        NONE,
        LINE_DISPLAYING,
        LINE_SWITCHING,
        SHUTTLE_WAIT_START,
        SHUTTLING_LEFT,
        SHUTTLE_PAUSED_IDLE,
        SHUTTLING_RIGHT,
        SHUTTLE_END_IDLE
    };
    int mUpdateInterval;
    int mSwitchInterval;
    int mShuttleInterval;
    QMutex mMutex;
    bool mSwitchOnClick;
    bool mSwitchEnd;
    State mState;
    int mTimerID;
    int mIdleTime;
    int mOffsetX, mOffsetY;
    QStringList mStringList;
    QList<int> mLineWidths;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TSCROLLLABEL_H
