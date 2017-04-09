#include "utils.h"

#include <QtMath>

QString Utils::microSecToTimeStr(long ms, bool padZero)
{
    return secToTimeStr(qCeil((double)ms/1000), padZero);
}

QString Utils::secToTimeStr(long seconds, bool padZero)
{
    int hour = seconds / 3600;
    if(hour > 99)
        hour = 99;

    seconds %= 3600;

    int minute = seconds / 60;
    if(minute>99)
        minute = 99;

    seconds %= 60;

    int second = seconds % 60;

    QString result;
    if(hour > 0)
        result = QString::asprintf("%2d:%2d:%2d", hour, minute, second);
    else
        result = QString::asprintf("%2d:%2d", minute, second);

    if(padZero)
        result = result.replace(" ", "0");

    return result;
}

QString Utils::absoluteFilePath(QString fileName)
{
    QFileInfo fi(fileName);
    if(fi.isRelative())
    {
        QDir dir(qApp->applicationDirPath());
        fileName = dir.absoluteFilePath(fileName);
    }
    return fileName;
}
