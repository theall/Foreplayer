#ifndef GUIADAPTER_H
#define GUIADAPTER_H

#include "legacy/guimanager.h"

class TGuiProxy : public QObject
{
public:
    explicit TGuiProxy(QObject *parent = 0);

    void exit();
    void open();
    void restoreGui();
    void setTitle(QString title);

    TGuiManager *getGuiManager();

private:
    TGuiManager *mGuiManager;
};

#endif // GUIADAPTER_H
