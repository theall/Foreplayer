#ifndef TSCROLLBAR_H
#define TSCROLLBAR_H

#include "style/scrollbarstyle.h"

class TScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit TScrollBar(QWidget *parent = 0);
    explicit TScrollBar(Qt::Orientation orientation, QWidget *parent=0);

    ~TScrollBar();

    static void setPixmaps(QPixmap groove, QPixmap up, QPixmap down, QPixmap handle);
    static void setPixmaps(QPixmap groove, QPixmap buttons, QPixmap handle);

    const TButtonPixmap *handlePixmap() const;
    const TButtonPixmap *upPixmap() const;
    const TButtonPixmap *downPixmap() const;
    const QPixmap *groovePixmap() const;

signals:
    void onVisibleToggle(bool visible);

protected:
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *) Q_DECL_OVERRIDE;

private:
    static QPixmap *mGroovePixmap;
    static TButtonPixmap *mUpPixmap;
    static TButtonPixmap *mDownPixmap;
    static TButtonPixmap *mHandlePixmap;
    static int mFixWidth;
    TScrollbarStyle mStyle;

    void initialize();
};
#endif // TSCROLLBAR_H
