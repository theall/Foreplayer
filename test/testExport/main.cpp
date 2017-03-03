#include <QCoreApplication>

#include <QProcess>
#include <QSharedMemory>
#include <QUuid>
#include "../../app/utils/utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString strId = QUuid::createUuid().toString();
    QSharedMemory mem(strId);
    TExportParam param;
    wcscpy(param.fileName, L"z:\\spc\\mmax2.rsn");
    wcscpy(param.indexName, L"mmax2-05.spc");
    wcscpy(param.outputPath, L"z:\\mp3test\\mmax2-05.mp3");
    param.overwrite = true;
    mem.create(sizeof(TExportParam));
    mem.attach();
    memcpy(mem.data(), &param, sizeof(param));

    TExportParam *p = (TExportParam *)mem.data();
    QProcess::startDetached("exportor "+strId);
    printf(strId.toLocal8Bit().constData());
    p->state = ES_RUN;
    while (true) {
        QThread::msleep(1000);
        //mem.lock();
        if(p->progressTotalFrames>0 && p->progressCurrentFrames<=p->progressTotalFrames)
            printf("%d/%d\n", p->progressCurrentFrames, p->progressTotalFrames);
        //mem.unlock();
    }

    return a.exec();
}

