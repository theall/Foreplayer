#ifndef TSDLFRONT_H
#define TSDLFRONT_H

#include "abstractfront.h"

class TSDLFront : public TAbstractFront
{
public:
    explicit TSDLFront();
    ~TSDLFront();

    bool start();
    void stop();
    void step();

    void play();
    void pause();

    void setRecorder();
    void setFilter();

    void setSampleSize(int sampleSize);

private:
};

#endif // TSDLFRONT_H
