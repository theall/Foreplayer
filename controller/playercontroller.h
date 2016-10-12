#ifndef TPLAYERCONTROLLER_H
#define TPLAYERCONTROLLER_H

#include <QObject>

class TPlayerController : public QObject
{
    Q_OBJECT
public:
    explicit TPlayerController(QObject *parent = 0);

    void joint();

signals:

public slots:
};

#endif // TPLAYERCONTROLLER_H
