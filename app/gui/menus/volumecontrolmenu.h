#ifndef TVOLUMECONTROLMENU_H
#define TVOLUMECONTROLMENU_H

#include "abstractmenu.h"

class TVolumeControlMenu : public TAbstractMenu
{
    Q_OBJECT

public:
    TVolumeControlMenu(QWidget *parent = 0);
    ~TVolumeControlMenu();

signals:
    void onVolumeUpTriggered();
    void onVolumeDownTriggered();
    void onVolumeMuteTriggered();

private:
    QAction *mActionVolumeUp;
    QAction *mActionVolumeDown;
    QAction *mActionVolumeMute;

    // TAbstractMenu interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};



#endif // TVOLUMECONTROLMENU_H
