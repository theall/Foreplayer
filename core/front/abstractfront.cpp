#include "abstractfront.h"


TAbstractFront::TAbstractFront() :
    mCallback(NULL)
{

}

void TAbstractFront::setCallback(TCallback callback)
{
    mCallback = callback;
}

void TAbstractFront::requestNextSamples(int n, short *samples)
{
    if(mCallback)
        mCallback(n, samples);
}
