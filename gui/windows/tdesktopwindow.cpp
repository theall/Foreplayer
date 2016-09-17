#include "tdesktopwindow.h"

TDesktopWindow::TDesktopWindow(QWidget *parent) :
    TAbstractWindow(parent),
    mDesktop(QApplication::desktop())
{
    setObjectName("DesktopWindow");

    mScreen = mDesktop->primaryScreen();
    connect(mDesktop, SIGNAL(resized(int)), this, SLOT(slotScreenResized(int)));

    updateGeometry();
}

TDesktopWindow::~TDesktopWindow()
{
    delete mDesktop;
}

void TDesktopWindow::slotScreenResized(int screen)
{
    if(mScreen==screen)
    {
        updateGeometry();
    }
}

void TDesktopWindow::updateGeometry()
{
    setGeometry(mDesktop->availableGeometry(mScreen));
}


void TDesktopWindow::retranslateUi()
{
}
