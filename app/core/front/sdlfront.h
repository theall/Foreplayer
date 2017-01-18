#ifndef TSDLFRONT_H
#define TSDLFRONT_H

#include "abstractfront.h"

class TSDLFront : public TAbstractFront
{
public:
    explicit TSDLFront();
    ~TSDLFront();

    void setRecorder();
    void setFilter();
    void start();
    void stop();
    void step();
};

#endif // TSDLFRONT_H
