#ifndef TABSTRACTFRONT
#define TABSTRACTFRONT

#include "../plugins/backend/backendinterface.h"

class TAbstractFront
{
public:
    TAbstractFront();

    virtual void setRecorder() = 0;
    virtual void setFilter() = 0;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void step() = 0;

    void setCallback(TRequestSamples callback);
    void requestNextSamples(int size, short *samples);

private:
    TRequestSamples mCallback;
};

#endif // TABSTRACTFRONT
