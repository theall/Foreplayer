#ifndef UTILS_H
#define UTILS_H

#include "pch.h"

namespace Utils{
    const int TextHighlight = Qt::UserRole + 1;
    const int IsCurrentRow = TextHighlight + 1;

    QString microSecToTimeStr(long ms, bool padZero=true);
    QString secToTimeStr(long seconds, bool padZero=true);
}

#endif // UTILS_H
