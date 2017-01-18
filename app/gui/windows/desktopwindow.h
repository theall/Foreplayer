#ifndef TDESKTOPWINDOW_H
#define TDESKTOPWINDOW_H

#include "abstractwindow.h"

class TDesktopWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TDesktopWindow(QWidget *parent = 0);
    ~TDesktopWindow();

private slots:
    void slotScreenResized(int);

private:
    int mScreen;
    QDesktopWidget *mDesktop;
    void updateGeometry();

    // TAbstractWindow interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TDESKTOPWINDOW_H
