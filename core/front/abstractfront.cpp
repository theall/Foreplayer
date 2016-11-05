#include "abstractfront.h"


TAbstractFront::TAbstractFront() :
    mCallback(NULL)
{

}

TAbstractFront::~TAbstractFront()
{

}

void TAbstractFront::setCallback(TRequestSamples callback)
{
    mCallback = callback;
}

void TAbstractFront::requestNextSamples(int n, short *samples)
{
    if(mCallback)
        mCallback(n, samples);
}
