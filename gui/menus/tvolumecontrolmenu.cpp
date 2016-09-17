#include "tvolumecontrolmenu.h"

TVolumeControlMenu::TVolumeControlMenu(QWidget *parent) :
    TAbstractMenu(parent)
{
    mActionVolumeUp = new QAction(this);
    mActionVolumeDown = new QAction(this);
    mActionVolumeMute = new QAction(this);

    connect(mActionVolumeUp, SIGNAL(triggered()), this, SIGNAL(onVolumeUpTriggered()));
    connect(mActionVolumeDown, SIGNAL(triggered()), this, SIGNAL(onVolumeDownTriggered()));
    connect(mActionVolumeMute, SIGNAL(triggered()), this, SIGNAL(onVolumeMuteTriggered()));

    addAction(mActionVolumeUp);
    addAction(mActionVolumeDown);
    addSeparator();
    addAction(mActionVolumeMute);

    retranslateUi();
}

void TVolumeControlMenu::retranslateUi()
{
    mActionVolumeUp->setText(tr("Up"));
    mActionVolumeDown->setText(tr("Down"));
    mActionVolumeMute->setText(tr("Mute"));
}
